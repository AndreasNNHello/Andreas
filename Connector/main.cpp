#include <iostream>
#include <curl/curl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdexcept>
#include <xlnt/xlnt.hpp>

std::string escape(CURL *curl_handle, const std::string& text)
{
    std::string result;
    char* esc_text= curl_easy_escape(curl_handle, text.c_str(), text.length());
    if(!esc_text) throw std::runtime_error("Can not convert string to URL");

    result = esc_text;
    curl_free(esc_text);

    return result;
}

static size_t write_data(char *ptr, size_t size, size_t nmemb, std::string* data)
{
    if (data)
    {
        data->append(ptr, size*nmemb);
        return size*nmemb;
    }
    else return 0;  // будет ошибка
}

static size_t write_head(char *ptr, size_t size, size_t nmemb, std::ostream* stream)
{
    (*stream)<< std::string(ptr, size*nmemb);
    return size*nmemb;
}

int main()
{
    /*// Пользовательские данные
    const std::string url_dologin= "https://zakupki360.ru/Account/Login?";// Authentication
    const std::string url_user= "https://zakupki360.ru/Search"; // Site

    std::string user_name;  //  Email
    std::string password;   //  Password

    std::cout<<"Email: ";
    getline(std::cin,user_name);
    std::cout<<password<<"Password: ";
    getline(std::cin,password);
    std::cout<<std::endl;
// andrushkann0815@gmail.com  myPassword1234
    CURL *curl_handle = curl_easy_init();
    if(curl_handle)
    {
        // Формирование  запроса на основе пользовательских данных
        std::string post_data;
        post_data+= "Email="  + escape(curl_handle, user_name);
        post_data+= "&Password=" + escape(curl_handle, password);
//        post_data+= "&RememberMe=true";
        post_data+= "&ReturnUrl=%2FSearch";

        curl_easy_setopt(curl_handle, CURLOPT_URL, url_dologin.c_str() );

        //  сохраняем html код cтраницы в строку content
        std::string content;
        curl_easy_setopt(curl_handle, CURLOPT_WRITEFUNCTION, write_data);
        curl_easy_setopt(curl_handle, CURLOPT_WRITEDATA,     &content);

        // Загловок ответа сервера выводим в консоль
        curl_easy_setopt(curl_handle, CURLOPT_HEADERFUNCTION, write_head);
        curl_easy_setopt(curl_handle, CURLOPT_WRITEHEADER,    &std::cout);

        // авто перенаправление
        curl_easy_setopt(curl_handle, CURLOPT_FOLLOWLOCATION, 1);
        // max 5-ть перенаправлений
        curl_easy_setopt(curl_handle, CURLOPT_MAXREDIRS, 5);
        // разрешаем использовать куки
        curl_easy_setopt(curl_handle, CURLOPT_COOKIEFILE,"");

        // POST- запрос c авторизацией ( происходит получение кукисов )
        curl_easy_setopt(curl_handle, CURLOPT_POSTFIELDS, post_data.c_str() );
        curl_easy_setopt(curl_handle, CURLOPT_POSTFIELDSIZE, post_data.length() );

        CURLcode res = curl_easy_perform(curl_handle);
        if(res != CURLE_OK)
        {
            std::cout<< curl_easy_strerror(res) << std::endl;
            getchar();
            return 0;
        }
        // Проверяем успешно ли авторизировались
        if( content.find("EmployeesAndCompanies") != std::string::npos )
            std::cout << "Authenticated!" <<std::endl<<std::endl;
        else
        {
            std::cout<< "Non-Authenticated!" <<std::endl;
            curl_easy_cleanup(curl_handle);
            getchar();
            return 0;
        }
        // GET- запрос для перехода в Search
        content.clear();
        // меняем POST-режим на GET
        curl_easy_setopt(curl_handle, CURLOPT_POST, 0);
        curl_easy_setopt(curl_handle, CURLOPT_HTTPGET, 1);
        // меняем url
        curl_easy_setopt(curl_handle, CURLOPT_URL, url_user.c_str() );

        res = curl_easy_perform(curl_handle);
        if(res != CURLE_OK)  std::cout<< curl_easy_strerror(res) <<std::endl;

        // Проверяем получили то что ожидали
        if( content.find("Search") != std::string::npos){
            std::cout << "Search page downloaded"  <<std::endl
                                                     << "------------- Content -----------------" <<std::endl
                                                     << content <<std::endl;
        }
        else  std::cout << "Is not Search page" <<std::endl;

        curl_easy_cleanup(curl_handle);
    }*/

//    getchar();
    return 0;
}