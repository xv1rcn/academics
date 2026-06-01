public final class Complex {
    private final double re;
    private final double im;

    public Complex(double re, double im) {
        this.re = re;
        this.im = im;
    }

    public double getReal() {
        return re;
    }

    public double getImag() {
        return im;
    }

    public Complex add(Complex other) {
        return new Complex(this.re + other.re, this.im + other.im);
    }

    public Complex sub(Complex other) {
        return new Complex(this.re - other.re, this.im - other.im);
    }

    public Complex mul(Complex other) {
        return new Complex(
            this.re * other.re - this.im * other.im,
            this.re * other.im + this.im * other.re
        );
    }

    public Complex div(Complex other) {
        double denominator = other.re * other.re + other.im * other.im;
        return new Complex(
            (this.re * other.re + this.im * other.im) / denominator,
            (this.im * other.re - this.re * other.im) / denominator
        );
    }

    @Override
    public String toString() {
        if (im == 0) {
            return String.format("%f", re);
        } else if (re == 0) {
            return String.format("%fi", im);
        } else if (im > 0) {
            return String.format("%f+%fi", re, im);
        } else {
            return String.format("%f-%fi", re, -im);
        }
    }
}
