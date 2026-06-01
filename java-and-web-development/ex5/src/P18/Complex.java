import java.io.Serializable;

public class Complex implements Serializable {
    private static final long serialVersionUID = 1L;
    private double real;
    private double imag;

    public Complex(double real, double imag) {
        this.real = real;
        this.imag = imag;
    }

    @Override
    public String toString() {
        if (imag >= 0) {
            return String.format("%.1f+%.1fi", real, imag);
        } else {
            return String.format("%.1f%.1fi", real, imag);
        }
    }
}
