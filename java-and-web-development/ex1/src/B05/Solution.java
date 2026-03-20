public class Solution {
    private static boolean[] visited = new boolean[10];
    private static int[] permutation = new int[9];

    private static void dfs(int position) {
        if (position == 9) {
            int firstRatio = permutation[0] * 100 + permutation[1] * 10 + permutation[2];
            int secondRatio = permutation[3] * 100 + permutation[4] * 10 + permutation[5];
            if (firstRatio * 2 != secondRatio) {
                return;
            }
            int thirdRatio = permutation[6] * 100 + permutation[7] * 10 + permutation[8];
            if (firstRatio * 3 == thirdRatio) {
                System.out.printf("%d:%d:%d=1:2:3\n", firstRatio, secondRatio, thirdRatio);
            }
            return;
        }
        for (int number = 1; number <= 9; ++number) {
            if (visited[number]) {
                continue;
            }
            visited[number] = true;
            permutation[position] = number;
            dfs(position + 1);
            visited[number] = false;
        }
    }

    public static void main(String[] args) {
        dfs(0);
    }
}
