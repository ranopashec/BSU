import java.util.*;

public class WordSorter {
    public static void main(String[] args) {
        Scanner scanner = new Scanner(System.in);
        Map<String, List<Integer>> wordMap = new HashMap<>();
        int lineNumber = 1;

        while (scanner.hasNextLine()) {
            String line = scanner.nextLine();
            String[] words = line.split("\\s+");

            for (String word : words) {
                if (word.matches("-?\\d+")) {
                    addWordToMap(wordMap, "Integer", word, lineNumber);
                } else if (word.matches("-?0x[0-9a-fA-F]+")) {
                    addWordToMap(wordMap, "Hexadecimal", word, lineNumber);
                } else if (word.matches("-?\\d+\\.\\d+")) {
                    addWordToMap(wordMap, "Float", word, lineNumber);
                } else if (word.matches("-?\\d+(\\.\\d+)?[eE][+-]?\\d+")) {
                    addWordToMap(wordMap, "Exponential", word, lineNumber);
                } else {
                    addWordToMap(wordMap, "Text", word, lineNumber);
                }
            }

            lineNumber++;
        }

        printSortedWords(wordMap);
    }

    private static void addWordToMap(Map<String, List<Integer>> wordMap, String type, String word, int lineNumber) {
        List<Integer> lineNumbers = wordMap.computeIfAbsent(type, k -> new ArrayList<>());
        lineNumbers.add(lineNumber);
    }

    private static void printSortedWords(Map<String, List<Integer>> wordMap) {
        List<Integer> integerNumbers = wordMap.getOrDefault("Integer", Collections.emptyList());
        Collections.sort(integerNumbers, Collections.reverseOrder());
        printWords("Integer", integerNumbers);

        List<Integer> hexadecimalNumbers = wordMap.getOrDefault("Hexadecimal", Collections.emptyList());
        Collections.sort(hexadecimalNumbers, Collections.reverseOrder());
        printWords("Hexadecimal", hexadecimalNumbers);

        List<Integer> floatNumbers = wordMap.getOrDefault("Float", Collections.emptyList());
        Collections.sort(floatNumbers);
        printWords("Float", floatNumbers);

        List<Integer> exponentialNumbers = wordMap.getOrDefault("Exponential", Collections.emptyList());
        Collections.sort(exponentialNumbers);
        printWords("Exponential", exponentialNumbers);

        List<String> textWords = wordMap.getOrDefault("Text", Collections.emptyList()).stream()
                .map(Object::toString)
                .sorted(Collections.reverseOrder())
                .toList();
        printWords("Text", textWords);
    }

    private static void printWords(String type, List<?> words) {
        System.out.println(type + ":");
        for (Object word : words) {
            System.out.println(word);
        }
        System.out.println();
    }
}
