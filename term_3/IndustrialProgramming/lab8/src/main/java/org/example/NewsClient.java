package org.example;
import java.io.*;
import java.net.Socket;
import java.util.ArrayList;
import java.util.Date;
import java.util.List;

public class NewsClient {
    private static final String SERVER_ADDRESS = "localhost";
    private static final int SERVER_PORT = 12345;
    private Socket socket;
    private ObjectOutputStream out;
    private ObjectInputStream in;

    public NewsClient() throws IOException {
        socket = new Socket(SERVER_ADDRESS, SERVER_PORT);
        out = new ObjectOutputStream(socket.getOutputStream());
        in = new ObjectInputStream(socket.getInputStream());
    }

    public void sendNews(News news) throws IOException {
        out.writeObject(news);
        out.flush();
    }

    public void requestNewsByDate(Date date) throws IOException, ClassNotFoundException {
        out.writeObject(date);
        out.flush();
        Object response = in.readObject();

        if (response instanceof ArrayList) {
            List<News> newsList = (List<News>) response;
            for (News news : newsList) {
                System.out.println(news);
            }
        } else if (response instanceof News) {
            System.out.println(response);
        } else {
            System.out.println("Unexpected response from server: " + response);
        }
    }





    public static void main(String[] args) throws IOException, ClassNotFoundException, InterruptedException {
        NewsClient client = new NewsClient();

        // Отправка новости
        client.sendNews(new News("Breaking News", "Something happened!", new Date()));

        Thread.sleep(500);

        // Запрос новостей по дате
        client.requestNewsByDate(new Date());
    }

}
