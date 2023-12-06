fn main() {
    let mut sum = 0;
    let lines = std::io::stdin()
        .lines()
        .map(|result| result.unwrap());
    for line in lines {
        let numeric_table = [
            ["0", "zero"],
            ["1", "one"],
            ["2", "two"],
            ["3", "three"],
            ["4", "four"],
            ["5", "five"],
            ["6", "six"],
            ["7", "seven"],
            ["8", "eight"],
            ["9", "nine"],
        ];
        let mut left_position = isize::MAX;
        let mut left_digit = 0;
        let mut right_position = -1;
        let mut right_digit = 0;
        for (i, row) in numeric_table.iter().enumerate() {
            for string in row {
                if let Some(position) = line.find(string) {
                    if (position as isize) < left_position {
                        left_position = position as isize;
                        left_digit = i;
                    }
                }
                if let Some(position) = line.rfind(string) {
                    if (position as isize) > right_position {
                        right_position = position as isize;
                        right_digit = i;
                    }
                }
            }
        }
        sum += left_digit * 10 + right_digit;
    }
    println!("{sum}");
}
