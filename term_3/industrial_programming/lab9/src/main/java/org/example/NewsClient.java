package org.example;

import java.rmi.Naming;
import java.util.Date;
import java.util.List;

public class NewsClient {
    private NewsService newsService;

    public NewsClient() {
        try {
            newsService = (NewsService) Naming.lookup("rmi://localhost/NewsService");
        } catch (Exception e) {
            e.printStackTrace();
        }
    }

    public void sendNews(News news) {
        try {
            newsService.addNews(news);
            System.out.println("News sent: " + news);
        } catch (Exception e) {
            e.printStackTrace();
        }
    }

    public void requestNewsByDate(Date date) {
        try {
            List<News> newsList = newsService.getNewsByDate(date);
            for (News news : newsList) {
                System.out.println(news);
            }
        } catch (Exception e) {
            e.printStackTrace();
        }
    }

    public static void main(String[] args) {
        NewsClient client = new NewsClient();

        // Отправка новости
        client.sendNews(new News("Breaking News", "Something happened!", new Date()));

        // Запрос новостей по дате
        client.requestNewsByDate(new Date());
    }
}
