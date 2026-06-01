import java.util.ArrayList;

public class Main {
    public static void main(String[] args) {
        Library library = new Library("敬文图书馆");

        Book b1 = new Book("Java 编程思想", "Bruce Eckel", "9787111213826");
        Book b2 = new Book("深入理解Java虚拟机", "周志明", "9787115446664");
        Book b3 = new Book("Java 核心技术", "Cay Horstmann", "9787111699555");

        library.addBook(b1);
        library.addBook(b2);
        library.addBook(b3);

        library.displayBooks();

        library.removeBook("9787115446664");

        ArrayList<Book> result = library.searchByTitle("Java");
        for (Book book : result) {
            System.out.println(book);
        }

        library.displayBooks();
    }
}
