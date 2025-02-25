package org.example;

import java.rmi.RemoteException;
import java.rmi.server.UnicastRemoteObject;
import java.util.ArrayList;
import java.util.Calendar;
import java.util.Date;
import java.util.List;

public class NewsServerImpl extends UnicastRemoteObject implements NewsService {
    private List<News> newsList;

    protected NewsServerImpl() throws RemoteException {
        newsList = new ArrayList<>();
    }

    @Override
    public synchronized void addNews(News news) throws RemoteException {
        newsList.add(news);
        System.out.println("News added: " + news);
    }

    @Override
    public synchronized List<News> getNewsByDate(Date date) throws RemoteException {
        List<News> result = new ArrayList<>();
        for (News news : newsList) {
            if (isSameDay(news.getDate(), date)) {
                result.add(news);
            }
        }
        return result;
    }

    private boolean isSameDay(Date date1, Date date2) {
        Calendar cal1 = Calendar.getInstance();
        Calendar cal2 = Calendar.getInstance();
        cal1.setTime(date1);
        cal2.setTime(date2);
        return cal1.get(Calendar.YEAR) == cal2.get(Calendar.YEAR) &&
                cal1.get(Calendar.DAY_OF_YEAR) == cal2.get(Calendar.DAY_OF_YEAR);
    }

    public static void main(String[] args) {
        try {
            NewsService newsService = new NewsServerImpl();
            java.rmi.registry.LocateRegistry.createRegistry(1099);
            java.rmi.Naming.rebind("NewsService", newsService);
            System.out.println("News Server is ready.");
        } catch (Exception e) {
            e.printStackTrace();
        }
    }
}
