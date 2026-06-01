public enum DiscountType {
    NONE,
    PERCENTAGE,
    FIXED,
    THRESHOLD;

    public static DiscountStrategy createStrategy(String type, double... params) {
        DiscountType discountType;
        try {
            discountType = DiscountType.valueOf(type.toUpperCase());
        } catch (IllegalArgumentException e) {
            throw new IllegalArgumentException("不支持的优惠类型: " + type);
        }

        switch (discountType) {
            case NONE:
                return new NoDiscountStrategy();

            case PERCENTAGE:
                if (params.length != 1) {
                    throw new IllegalArgumentException("百分比折扣需要一个参数: 折扣率");
                }
                return new PercentageDiscountStrategy(params[0]);

            case FIXED:
                if (params.length != 1) {
                    throw new IllegalArgumentException("固定减免需要一个参数: 减免金额");
                }
                return new FixedAmountDiscountStrategy(params[0]);

            case THRESHOLD:
                if (params.length != 2) {
                    throw new IllegalArgumentException("满减需要两个参数: 门槛金额、减免金额");
                }
                return new ThresholdDiscountStrategy(params[0], params[1]);

            default:
                throw new IllegalArgumentException("未知的优惠类型: " + type);
        }
    }
}
