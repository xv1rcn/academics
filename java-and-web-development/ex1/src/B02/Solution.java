public class Solution {
    public static void main(String[] args) {
        int n = 0;
        while (n * n < 1_000) {
            ++n;
        }
        while (n * n < 10_000) {
            String nString = Integer.toString(n * n);
            if (nString.charAt(0) == nString.charAt(1) && nString.charAt(2) == nString.charAt(3)) {
                System.out.printf("%d\t", n * n);
            }
            ++n;
        }
    }
}
