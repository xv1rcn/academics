import java.util.ArrayList;

public class Library {
    private String name;
    private ArrayList<Book> bookList;
    
    public Library(String name) {
        this.name = name;
        bookList = new ArrayList<>();
    }

    public void addBook(Book book) {
        bookList.add(book);
        System.out.println("添加成功: " + book);
    }

    public void removeBook(String isbn) {
        for (Book book : bookList) {
            if (book.getIsbn().equals(isbn)) {
                bookList.remove(book);
                System.out.println("删除成功: " + book);
                return;
            }
        }
        System.out.println("未找到 ISBN: " + isbn + " 的书籍");
    }

    public ArrayList<Book> searchByTitle(String title) {
        ArrayList<Book> result = new ArrayList<>();
        for (Book book : bookList) {
            if (book.getTitle().contains(title)) {
                result.add(book);
            }
        }
        return result;
    }

    public void displayBooks() {
        if (bookList.isEmpty()) {
            System.out.println(name + " 暂无书籍");
            return;
        }
        System.out.println(name + " 的所有书籍: ");
        for (Book book : bookList) {
            System.out.println(book);
        }
    }
}
