public class Solution {
    public static void main(String[] args) {
        for (int row = 1; row <= 9; ++row) {
            for (int column = 1; column <= row; ++column) {
                System.out.printf("%d*%d=%-2d\t", row, column, row * column);
            }
            System.out.println();
        }
    }
}
