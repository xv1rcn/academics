public class Shield implements Armor {
    @Override
    public int defend() {
        return 8;
    }

    @Override
    public String getArmorType() {
        return "盾牌";
    }
}
