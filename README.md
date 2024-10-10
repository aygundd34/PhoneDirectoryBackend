# Phone Directory Backend 

**Phone Directory Backend**, C++ programlama dili ile geliştirilmiş bir telefon rehberi yönetim sistemidir. Bu proje, **Crow** framework'ü ile geliştirilmiş bir RESTful API sunarak PostgreSQL veritabanında kayıtları yönetmeyi sağlar. Kullanıcıların telefon numaralarını ekleyebilmesi, düzenleyebilmesi ve silebilmesi için temel CRUD (Create, Read, Update, Delete) işlemlerini destekler.

## 📌 Özellikler

-   **Kişi Ekleme:** Kullanıcılar, `/contact/add` endpoint'i aracılığıyla yeni kişileri rehbere kolayca ekleyebilirler.
-   **Kişi Listeleme:** `/contact/getlist` endpoint'i ile rehberdeki tüm kişilerin listesini alabilirsiniz.
-   **Kişi Bilgisi Alma:** `/contact/get?id={id}` endpoint'i ile belirli bir kişinin bilgilerine erişim sağlanır.
-   **Kişi Güncelleme:** `/contact/update` endpoint'i ile mevcut kişilerin bilgileri güncellenebilir.
-   **Kişi Silme:** `/contact/delete/{id}` endpoint'i kullanılarak gereksiz kişiler rehberden çıkarılabilir.
-   **PostgreSQL Tabanlı:** Verilerin güvenli ve verimli bir şekilde saklanması için PostgreSQL veritabanı kullanılır.

## 🛠 Teknolojiler

Bu projede kullanılan başlıca teknolojiler:

-   **C++:** Uygulamanın temel programlama dili.
-   **Crow Framework:** C++ için hızlı ve minimalist bir HTTP framework'ü.
-   **PostgreSQL:** Veritabanı yönetim sistemi.
-   **libpqxx:** PostgreSQL ile C++ üzerinden veritabanı bağlantısı sağlayan kütüphane.
-   **nlohmann/json:** JSON verilerini işlemek için kullanılan modern C++ kütüphanesi.

## 📥 Kurulum

Projeyi yerel ortamınızda çalıştırmak için aşağıdaki adımları izleyin:

1.  **Gereksinimler:**
    -   C++
    -   PostgreSQL
    -   Gerekli Bağımlılıklar:
        -   Crow
        -   libpqxx
        -   nlohmann/json

## 🚀 API Kullanımı

Aşağıdaki RESTful API endpoint'lerini kullanarak CRUD işlemlerini gerçekleştirebilirsiniz:

-   **POST /contact/add:** Yeni bir kişi ekleyin.
-   **GET /contact/getlist:** Tüm kişileri listeleyin.
-   **GET /contact/get?id={id}:** Belirli bir kişinin bilgilerini alın.
-   **PUT /contact/update:** Kayıtları güncelleyin.
-   **DELETE /contact/delete/{id}:** Kişiyi silin.

## 🚀 Kullanım

1.  **Kişi Ekleme:** Web arayüzünde "Contact Add" butonuna tıklayın ve gerekli bilgileri doldurun.
2.  **Kişi Listeleme:** Ana sayfada tüm kişilerin listesi görüntülenir.
3.  **Kişi Güncelleme:** Listeden bir kişinin yanındaki "Update" seçeneği ile bilgilerini güncelleyebilirsiniz.
4.  **Kişi Silme:** Listeden bir kişinin yanındaki "Delete" seçeneği ile o kişiyi rehberden silebilirsiniz.

## 🌐 Veritabanı İşlemleri

-   **Ekleme:** Yeni kişi eklemek için `/contact/add` route'unda JSON formatında gerekli bilgileri gönderin.
-   **Listeleme:** `/contact/getlist` ile veritabanındaki tüm kişileri listeleyin.
-   **Güncelleme:** `/contact/update` ile JSON formatında güncellemek istediğiniz kişinin bilgilerini gönderin.
-   **Silme:** `/contact/delete/{id}` route'u ile belirtilen ID'ye sahip kişiyi silin.
