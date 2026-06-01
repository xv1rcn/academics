public class Helmet implements Armor {
    @Override
    public int defend() {
        return 5;
    }

    @Override
    public String getArmorType() {
        return "头盔";
    }
}
