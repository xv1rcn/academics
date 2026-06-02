import java.util.List;

public class PageResult<T> {
    private List<T> data;
    private int totalRecords;
    private int totalPages;
    private int currentPage;

    public PageResult(List<T> data, int totalRecords, int totalPages, int currentPage) {
        this.data = data;
        this.totalRecords = totalRecords;
        this.totalPages = totalPages;
        this.currentPage = currentPage;
    }

    public List<T> getData() {
        return data;
    }

    public int getTotalRecords() {
        return totalRecords;
    }

    public int getTotalPages() {
        return totalPages;
    }

    public int getCurrentPage() {
        return currentPage;
    }
}
