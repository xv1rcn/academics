public class Solution {
    public static void main(String[] args) {
        int[] array = new int[4];
        for (array[0] = 4; array[0] <= 20; ++array[0]) {
            for (array[1] = 3; array[1] < array[0]; ++array[1]) {
                for (array[2] = 2; array[2] < array[1]; ++array[2]) {
                    for (array[3] = 1; array[3] < array[2]; ++array[3]) {
                        int numerator = array[1] * array[2] * array[3]
                                + array[0] * array[2] * array[3]
                                + array[0] * array[1] * array[3]
                                + array[0] * array[1] * array[2];
                        int denominator = array[0] * array[1] * array[2] * array[3];
                        if (numerator == denominator) {
                            for (int index = 0; index < 4; ++index) {
                                System.out.printf("%d\t", array[index]);
                            }
                            System.out.println("0");
                        }
                    }
                }
            }
        }
    }
}
