public class BattleTest {
    public static void main(String[] args) {
        System.out.println("=== 创建角色 ===");
        Character[] party = {
            new Warrior("亚瑟"),
            new Mage("梅林"),
            new Archer("罗宾")
        };

        party[0].equipWeapon(new Sword());
        party[0].equipArmor(new Chestplate());
        party[1].equipWeapon(new MagicStaff());
        party[1].equipArmor(new Helmet());
        party[2].equipWeapon(new Bow());
        party[2].equipArmor(new Shield());

        for (Character c : party) {
            c.printStatus();
        }

        System.out.println("\n=== 普通攻击（战士 → 法师） ===");
        party[0].attack(party[1]);
        party[1].printStatus();

        System.out.println("\n=== 战士释放技能后攻击 → 法师 ===");
        party[0].specialSkill();
        party[0].attack(party[1]);
        party[1].printStatus();

        System.out.println("\n=== 法师释放技能后攻击 → 战士 ===");
        party[1].specialSkill();
        party[1].attack(party[0]);
        party[0].printStatus();

        System.out.println("\n=== 弓箭手释放技能后攻击 → 战士 ===");
        party[2].specialSkill();
        party[2].attack(party[0]);
        party[0].printStatus();

        System.out.println("\n=== 全体状态 ===");
        for (Character c : party) {
            c.printStatus();
            System.out.println("是否存活: " + (c.isAlive() ? "是" : "否"));
        }
    }
}
