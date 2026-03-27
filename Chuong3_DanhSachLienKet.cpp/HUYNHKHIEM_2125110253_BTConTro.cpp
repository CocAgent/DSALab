#include <iostream>

using namespace std;



// Hàm hoán đổi dùng con trỏ

void swapTrucTiep(int* x, int* y) {

    int temp = *x; // Lấy giá trị tại địa chỉ x cất vào biến tạm

    *x = *y;       // Lấy giá trị tại địa chỉ y ghi đè vào địa chỉ x

    *y = temp;     // Ghi giá trị tạm vào địa chỉ y

}



int main() {

    int a = 3, b = 9;



    cout << "Truoc khi doi: a = " << a << ", b = " << b << endl;



    // Truyền "địa chỉ" của a và b vào hàm (dùng dấu &)

    swapTrucTiep(&a, &b);



    cout << "Sau khi doi:  a = " << a << ", b = " << b << endl;



    return 0;

}
//Truoc khi doi: a = 3, b = 9
//Sau khi doi : a = 9, b = 3
// Từ đoạn code trên cho em hiểu rõ hơn về con trỏ và còn thể tóm tắt nó bằng ngôn ngữ đời thường
//
//&a,Lấy địa chỉ,Hỏi: "Nhà thằng a nằm ở đâu?"
//int* x,Khai báo con trỏ,Tạo một tờ giấy chỉ để ghi địa chỉ nhà.
//*x,Giải tham chiếu,"Đến đúng địa chỉ ghi trên giấy, gõ cửa vào nhà lục đồ."
