public class Sword implements Weapon {
    @Override
    public int attack() {
        return 15;
    }

    @Override
    public String getWeaponType() {
        return "长剑";
    }
}
