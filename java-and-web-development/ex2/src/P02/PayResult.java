public enum PayResult {
    SUCCESS("支付成功"),
    FAILURE("支付失败");

    private final String message;

    PayResult(String message) {
        this.message = message;
    }

    public String getMessage() {
        return message;
    }
}
