import java.util.HashMap;
import java.util.Map;

public class Message {
    private String content;
    private Map<String, Object> headers;

    public Message(String content) {
        this.content = content;
        this.headers = new HashMap<>();
    }

    public String getContent() {
        return content;
    }

    public void setContent(String content) {
        this.content = content;
    }

    public Map<String, Object> getHeaders() {
        return headers;
    }

    public void setHeader(String key, Object value) {
        headers.put(key, value);
    }

    public Object getHeader(String key) {
        return headers.get(key);
    }
}
