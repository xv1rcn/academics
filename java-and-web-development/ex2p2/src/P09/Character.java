public abstract class Character {
    protected String name;
    protected int health;
    protected int mana;
    protected int baseAttack;
    protected Weapon weapon;
    protected Armor armor;

    public Character(String name, int health, int mana, int baseAttack) {
        this.name = name;
        this.health = health;
        this.mana = mana;
        this.baseAttack = baseAttack;
    }

    public String getName() {
        return name;
    }

    public int getHealth() {
        return health;
    }

    public int getMana() {
        return mana;
    }

    public void equipWeapon(Weapon w) {
        this.weapon = w;
    }

    public void equipArmor(Armor a) {
        this.armor = a;
    }

    public abstract void specialSkill();

    public int getTotalAttack() {
        int weaponAttack = (weapon != null) ? weapon.attack() : 0;
        return baseAttack + weaponAttack;
    }

    public int getTotalDefense() {
        return (armor != null) ? armor.defend() : 0;
    }

    public void takeDamage(int damage) {
        health = Math.max(0, health - damage);
    }

    public boolean isAlive() {
        return health > 0;
    }

    public void attack(Character target) {
        int rawDamage = getTotalAttack() - target.getTotalDefense();
        rawDamage = Math.max(1, rawDamage);
        int finalDamage = applySkillBonus(rawDamage);
        target.takeDamage(finalDamage);

        System.out.print(name + " 攻击了 " + target.getName());
        if (weapon != null) {
            System.out.print("（" + weapon.getWeaponType() + "）");
        }
        System.out.println("，造成 " + finalDamage + " 点伤害");

        if (!target.isAlive()) {
            System.out.println(target.getName() + " 已阵亡！");
        }
    }

    protected int applySkillBonus(int rawDamage) {
        return rawDamage;
    }

    public void printStatus() {
        String weaponName = (weapon != null) ? weapon.getWeaponType() : "无";
        String armorName = (armor != null) ? armor.getArmorType() : "无";
        System.out.printf(
            "%s [HP: %d | MP: %d | 武器: %s | 防具: %s]%n",
            name, health, mana, weaponName, armorName
        );
    }
}
