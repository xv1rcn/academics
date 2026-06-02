import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.io.ObjectInputStream;
import java.io.ObjectOutputStream;

public class BinaryTreeSerialize {
    public static void main(String[] args) {
        TreeNode root = new TreeNode("A");
        root.left = new TreeNode("B");
        root.right = new TreeNode("C");
        root.left.left = new TreeNode("D");
        root.right.left = new TreeNode("E");
        root.right.right = new TreeNode("F");
        root.right.left.left = new TreeNode("G");
        root.right.right.right = new TreeNode("H");

        try {
            ObjectOutputStream oos = new ObjectOutputStream(new FileOutputStream("tree.ser"));
            oos.writeObject(root);
            oos.close();

            ObjectInputStream ois = new ObjectInputStream(new FileInputStream("tree.ser"));
            TreeNode deserializedRoot = (TreeNode) ois.readObject();
            ois.close();

            System.out.printf("原树前序: %s%n", preOrder(root));
            System.out.printf("反序列化树前序: %s%n", preOrder(deserializedRoot));
        } catch (Exception e) {
            e.printStackTrace();
        }
    }

    public static String preOrder(TreeNode node) {
        if (node == null) {
            return "";
        }
        StringBuilder sb = new StringBuilder();
        sb.append(node.name);
        if (node.left != null) {
            sb.append(" ").append(preOrder(node.left));
        }
        if (node.right != null) {
            sb.append(" ").append(preOrder(node.right));
        }
        return sb.toString();
    }
}
