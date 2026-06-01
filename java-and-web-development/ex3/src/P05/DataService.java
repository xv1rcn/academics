import java.io.IOException;

public class DataService {
    public static String getDataFromFile(String filePath) throws DataAccessException {
        try {
            return FileUtil.readFileContent(filePath);
        } catch (IOException e) {
            throw new DataAccessException("数据访问失败: " + filePath, e);
        }
    }
}
