package org.example;
import java.io.Serializable;
import java.util.Date;

public class News implements Serializable {
    private static final long serialVersionUID = 1L;
    private String title;
    private String content;
    private Date date;

    public News(String title, String content, Date date) {
        this.title = title;
        this.content = content;
        this.date = date;
    }

    public String getTitle() {
        return title;
    }

    public String getContent() {
        return content;
    }

    public Date getDate() {
        return date;
    }

    @Override
    public String toString() {
        return "News{" +
                "title='" + title + '\'' +
                ", content='" + content + '\'' +
                ", date=" + date +
                '}';
    }
}
