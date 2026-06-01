public class MagicStaff implements Weapon {
    @Override
    public int attack() {
        return 12;
    }

    @Override
    public String getWeaponType() {
        return "法杖";
    }
}
