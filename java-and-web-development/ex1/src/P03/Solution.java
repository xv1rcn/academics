public class Solution {
    public static void main(String[] args) {
        byte b1 = (byte) 0xaa;
        byte b2 = (byte) 0xbb;
        byte b3 = (byte) 0xcc;
        byte b4 = (byte) 0xdd;

        int result = ((b4 & 0xff) << 24) | ((b3 & 0xff) << 16) | ((b2 & 0xff) << 8) | ((b1 & 0xff) << 0);
        System.out.printf("%d\t0x%s", result, Integer.toHexString(result));
    }
}
