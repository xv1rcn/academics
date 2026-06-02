import java.io.Serializable;

public class TreeNode implements Serializable {
    private static final long serialVersionUID = 1L;

    String name;
    TreeNode left;
    TreeNode right;

    public TreeNode(String name) {
        this.name = name;
        this.left = null;
        this.right = null;
    }
}
