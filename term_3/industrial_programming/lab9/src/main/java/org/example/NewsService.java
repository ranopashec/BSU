package org.example;

import java.rmi.Remote;
import java.rmi.RemoteException;
import java.util.Date;
import java.util.List;

public interface NewsService extends Remote {
    void addNews(News news) throws RemoteException;
    List<News> getNewsByDate(Date date) throws RemoteException;
}
