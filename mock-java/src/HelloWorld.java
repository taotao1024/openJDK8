import java.util.HashMap;

public class HelloWorld {
    public static void main(String[] args) {
        System.out.println("Hello, World!");
        HashMap<String, String> map = new HashMap<String, String>() {{
            this.put("1", "1");
            this.put("2", "2");
            this.put("3", "3");
        }};
        map.forEach((key, value) -> System.out.println("Key: " + key + ", Value: " + value));
    }
}