public class Bow implements Weapon {
    @Override
    public int attack() {
        return 10;
    }

    @Override
    public String getWeaponType() {
        return "长弓";
    }
}
