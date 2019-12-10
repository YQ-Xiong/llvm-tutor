//
// Created by Youyou Tu on 2019-12-04.
//

int main() {
    int i = 1;
    int j = 1;
    int k = 0;
    while (k < 5) {
        if (j < 3) {
            j = j + 1;
            k = k+1;
        }
        else {
            //j = k;
            k = k+2;
        }
    }
    return j;
}