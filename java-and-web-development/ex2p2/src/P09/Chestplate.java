public class Chestplate implements Armor {
    @Override
    public int defend() {
        return 10;
    }

    @Override
    public String getArmorType() {
        return "胸甲";
    }
}
