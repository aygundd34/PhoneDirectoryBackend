#include <crow.h>
#include <pqxx/pqxx>
#include <nlohmann/json.hpp>
#include <iostream>
using json = nlohmann::json;

int main() {
    crow::SimpleApp app;

    // EKLEME /contact/add rota
    CROW_ROUTE(app, "/contact/add").methods(crow::HTTPMethod::Post)
    ([](const crow::request &req) {
        try {
            // JSON verisini çözümleme
            auto json_data = json::parse(req.body);

            // JSON'dan veri alma ve doğrulama
            if (!json_data.contains("number") || !json_data.contains("name") ||
                !json_data.contains("surname") || !json_data.contains("mail")) {
                return crow::response{400, json::object({{"status", "error"}, {"message", "Eksik veri."}}).dump()};
            }

            auto number = json_data["number"].get<std::string>();
            auto name = json_data["name"].get<std::string>();
            auto surname = json_data["surname"].get<std::string>();
            auto mail = json_data["mail"].get<std::string>();

            // Veritabanı bağlantısı kurma
            pqxx::connection conn("dbname=phonebookdb user=postgres password=postgres hostaddr=127.0.0.1 port=5432");

            // Non-transaction başlatma
            pqxx::nontransaction txn(conn);

            // SQL INSERT komutunu tanımlama
            std::string sql = "INSERT INTO phonebookmanagment.phonebook (number, name, surname, mail) VALUES ($1, $2, $3, $4);";

            // SQL komutunu çalıştırma
            txn.exec_params(sql, number, name, surname, mail);

            // Başarı mesajı
            return crow::response{json::object({{"status", "success"}, {"message", "Kayıt başarıyla eklendi."}}).dump()};

        } catch (const std::exception& e) {
            // Hata durumunda yanıt
            return crow::response{500, json::object({{"status", "error"}, {"message", e.what()}}).dump()};
        }
    });


    // GETLİST /contact/get rota
    CROW_ROUTE(app, "/contact/getlist").methods(crow::HTTPMethod::Get)
    ([](const crow::request &req) {

        // Postgres Veritabanı bağlantısı kurma
        pqxx::connection conn("dbname=phonebookdb user=postgres password=postgres hostaddr=127.0.0.1 port=5432");

        std::cout << "Veritabanına başarıyla bağlandı: " << conn.dbname() << std::endl;

        // Non-transaction başlatma
        pqxx::nontransaction txn(conn);

        // SQL SELECT komutunu tanımlama (Veri listesini çekmek için)
        std::string sql = "SELECT id, number, name, surname, mail FROM phonebookmanagment.phonebook";

        // SQL komutunu çalıştırma ve sonuçları alma
        pqxx::result res = txn.exec(sql);

        crow::json::wvalue json_response;
        json_response["data"] = crow::json::wvalue::list();
        // Sonuçları işleme ve listeleme

        for (int i = 0; i < res.size(); i++) {
            const auto row = res[i];
            crow::json::wvalue json_item;
            json_item["id"] = row["id"].as<int>();
            json_item["number"] = row["number"].as<std::string>();
            json_item["name"] = row["name"].as<std::string>();
            json_item["surname"] = row["surname"].as<std::string>();
            json_item["mail"] = row["mail"].as<std::string>();
            json_response["data"][i] = std::move(json_item);
        }

        return crow::response{json_response};
    });




    // GET /contact/get?id=3 rota
    CROW_ROUTE(app, "/contact/get").methods(crow::HTTPMethod::Get)
    ([](const crow::request &req) {
        auto id_str = req.url_params.get("id");

        if (!id_str) {
            return crow::response(400, crow::json::wvalue({{"error", "id parametresi gerekli."}}).dump());
        }

        int id = std::stoi(id_str);
        pqxx::connection conn("dbname=phonebookdb user=postgres password=postgres hostaddr=127.0.0.1 port=5432");

        std::cout << "Veritabanına başarıyla bağlandı: " << conn.dbname() << std::endl;

        pqxx::nontransaction txn(conn);

        pqxx::result res = txn.exec(
            "SELECT id, number, name, surname, mail FROM phonebookmanagment.phonebook WHERE id = " + txn.quote(id));

        if (res.empty()) {
            return crow::response(404, crow::json::wvalue({{"error", "Kayıt bulunamadı."}}).dump());
        }

        const auto row = res[0];
        crow::json::wvalue json_item{
            {"id", row["id"].as<int>()},
            {"number", row["number"].as<std::string>()},
            {"name", row["name"].as<std::string>()},
            {"surname", row["surname"].as<std::string>()},
            {"mail", row["mail"].as<std::string>()}
        };

        return crow::response(200, crow::json::wvalue({{"data", std::move(json_item)}}).dump());
    });





    // UPDATE /contact/update rota (Kayıt güncellemek için)
    CROW_ROUTE(app, "/contact/update").methods(crow::HTTPMethod::Put)
    ([](const crow::request &req) {
        // JSON verisini çözümleme
        auto json_data = json::parse(req.body);

        // JSON'dan veri alma
        int id = json_data["id"].get<int>();
        auto number = json_data["number"].get<std::string>();
        auto name = json_data["name"].get<std::string>();
        auto surname = json_data["surname"].get<std::string>();
        auto mail = json_data["mail"].get<std::string>();

        // Postgres Veritabanı bağlantısı kurma
        pqxx::connection conn("dbname=phonebookdb user=postgres password=postgres hostaddr=127.0.0.1 port=5432");

        std::cout << "Veritabanına başarıyla bağlandı: " << conn.dbname() << std::endl;

        // Non-transaction başlatma
        pqxx::nontransaction txn(conn);

        // İlk olarak, ID ile kayıt olup olmadığını kontrol etmek gerekir. ID si olmayan birinin güncellenmeye çalışmasının önüne geçilir.
        std::string check_sql = "SELECT id FROM phonebookmanagment.phonebook WHERE id = $1;";
        pqxx::result check_result = txn.exec_params(check_sql, id);

        // Eğer kayıt yoksa, uygun hata mesajı döndür
        if (check_result.empty()) {
            return crow::response{
                json::object({{"status", "error"}, {"message", "Bu ID' ye sahip kayıt bulunmadığından güncelleme işlemi yapılamadı."}}).dump()
            };
        }

        // Kayıt varsa, güncelleme sorgusunu çalıştır
        std::string update_sql = "UPDATE phonebookmanagment.phonebook "
                "SET number = $2, name = $3, surname = $4, mail = $5 "
                "WHERE id = $1;";
        txn.exec_params(update_sql, id, number, name, surname, mail);

        // Başarı mesajı döndürme
        return crow::response{json::object({{"status", "success"}, {"message", "Kayıt başarıyla güncellendi."}}).dump()};
    });


    // DELETE /contact/delete/<id> rota
    CROW_ROUTE(app, "/contact/delete/<int>")
    .methods(crow::HTTPMethod::Delete)
    ([](int id) {
        // Postgres Veritabanı bağlantısı kurma
        pqxx::connection conn("dbname=phonebookdb user=postgres password=postgres hostaddr=127.0.0.1 port=5432");

        std::cout << "Veritabanına başarıyla bağlandı: " << conn.dbname() << std::endl;

        // Non-transaction başlatma
        pqxx::nontransaction txn(conn);

        // İlk olarak, ID ile kayıt olup olmadığını kontrol etme
        std::string check_sql = "SELECT id FROM phonebookmanagment.phonebook WHERE id = $1;";
        pqxx::result check_result = txn.exec_params(check_sql, id);

        // Eğer kayıt yoksa, uygun hata mesajı döndür
        if (check_result.empty()) {
            return crow::response{json::object({{"status", "error"}, {"message", "Bu ID'ye sahip kayıt bulunmadığından silme işlemi yapılamadı"}}).dump()};
        }

        // Kayıt varsa, silme sorgusunu çalıştır
        std::string delete_sql = "DELETE FROM phonebookmanagment.phonebook WHERE id = $1;";
        txn.exec_params(delete_sql, id);

        // Başarı mesajı döndürme
        std::cout << "Kayıt başarıyla silindi." << std::endl;
        return crow::response{json::object({{"status", "success"}, {"message", "Kayıt başarıyla silindi"}}).dump()};
    });

    app.port(18080).run();

    return 0;
}