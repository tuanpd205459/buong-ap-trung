****ĐẠI HỌC BÁCH KHOA HÀ NỘI****

**BÀI TẬP LỚN**

**Kỹ thuật lập trình trong Cơ điện tử** 


![Giao diện chính]([https://github.com/tuanpd205459/buong-ap-trung/blob/main/giao-dien-qt.png?raw=true])


**MỤC LỤC**
#
[**CHƯƠNG 1. GIỚI THIỆU ĐỀ TÀI	2****](#_toc141495606)**

[**CHƯƠNG 2. MÔ HÌNH HỆ THỐNG	3****](#_toc141495607)

[2.1	Các linh kiện được sử dụng	3](#_toc141495608)

[2.2	Sơ đồ đấu nối	5](#_toc141495609)

[2.3	Thông số linh kiện	6](#_toc141495610)

[2.3.1	Mạch Arduino R3	6](#_toc141495611)

[2.3.2	Cảm biến nhiệt độ, độ ẩm DHT11	7](#_toc141495612)

[2.3.3	Còi chíp 5V	7](#_toc141495613)

[2.3.4	Led 5mm	7](#_toc141495614)

[2.3.5	Màn hình LCD 1602	8](#_toc141495615)

[2.3.6	Motor 3V	8](#_toc141495616)

[2.3.7	Rơ-le 5VDC	9](#_toc141495617)

[**CHƯƠNG 3. NGUYÊN LÍ HOẠT ĐỘNG CỦA HỆ THỐNG	10****](#_toc141495618)

[**CHƯƠNG 4. LẬP TRÌNH CHO HỆ THỐNG	11****](#_toc141495619)

[4.1	Code Qt	11](#_toc141495620)

[4.2	Code Arduino	20](#_toc141495621)

[**CHƯƠNG 5. KẾT LUẬN	27****](#_toc141495622)

[**DANH MỤC HÌNH VẼ	28****](#_toc141495623)

[**DANH MỤC BẢNG BIỂU	29****](#_toc141495624)





15

1. # <a name="_toc79515938"></a><a name="_toc141495606"></a>**GIỚI THIỆU ĐỀ TÀI**
Buồng ấp trứng tự động là một hệ thống gồm các thiết bị có nhiệm vụ cảm biến và theo dõi trạng thái của buồng ấp; thông báo qua cơ cấu đèn LED và còi; hiển thị trạng thái nhiệt độ, độ ẩm, thời gian,..lên màn hình. 

Một hệ thống gồm tập hợp các thiết bị có nhiệm vụ phát hiện, thông báo và hiển thị trạng thái của buồng ấp trứng. Việc phát hiện ra nhiệt độ và độ ẩm có thể được thực hiện tự động bởi các thiết bị cảm biến. Hệ thống sẽ tự động phát hiện ra nhiệt độ và độ ẩm một cách nhanh chóng, chính xác và kịp thời trong vùng hệ thống đang đặt cảm biến. Tự động phát ra các tín hiệu báo động, chỉ thị và các tín hiệu điều khiển các thiết bị ngoại vi của hệ thống ấp trứng tự động nhằm thực hiện một nhiệm vụ cụ thể nào đó. Đặc biệt, với hệ thống buồng ấp trứng tự động sử dụng cảm biến nhiệt độ, độ ẩm thì nó còn có nhiệm vụ quan trọng hơn là thông báo và hiển thị những thông số trạng thái.

Nông nghiệp luôn đóng vai trò quan trọng trong đời sống con người. Với sự gia tăng dân số và nhu cầu về thực phẩm ngày càng tăng cao, việc cải thiện hiệu suất và năng suất trong ngành nông nghiệp trở nên cực kỳ cần thiết. Trong lĩnh vực chăn nuôi, quá trình ấp trứng đóng một vai trò then chốt để duy trì và phát triển đàn gia cầm. Nhận thấy cần thiết phải tạo ra một hệ thống giúp nâng cao hiệu suất và tối ưu hóa quá trình ấp trứng  nên nhóm chúng em quyết định nghiên cứu về đề tài hệ thống báo cháy đơn giản có quy mô hộ gia đình.











1. # <a name="_toc79515939"></a><a name="_toc141495607"></a>**MÔ HÌNH HỆ THỐNG**
   1. ## <a name="_toc141495608"></a>**Các linh kiện được sử dụng**
- Vi điều khiển Arduino Uno R3
- Module tích hợp cảm biến đo nhiệt độ, độ ẩm DHT11
- Màn hình LCD 16x2
- Rơ-le 5VDC
- Còi Buzzer báo động
- Motor 3V
- Đèn LED















































1. ## <a name="_toc141495609"></a>**Sơ đồ đấu nối**


|**Arduino Uno**|**Cảm biến nhiệt độ, độ ẩm DHT11**|**Còi chip 5V**|**LCD 1602**|**RedLed**|**GreenLed**|**Relay**|
| :-: | :-: | :-: | :-: | :-: | :-: | :-: |
|**5V**|VCC||VDD||||
|**GND**|GND||||||
|**11**|DATA||||||
|**2/RXD**|||||||
|**3/TXD**|||||||
|**A4**|||SDA||||
|**A5**|||SCL||||
|**13**||||**+**||**+**|
|**12**||**+**|||||
|**7**|||||**+**||
|**GND**||**-**|VSS|**-**|**-**|**-**|

<a name="_toc79523417"></a>                                                          *Bảng 1. Sơ đồ đấu nối*


<a name="_toc79516047"></a><a name="_toc79523559"></a>*Hình 5. Sơ đồ mạch*

1. ## <a name="_toc141495610"></a>**Thông số linh kiện**
   1. #### <a name="_toc141495611"></a>**Mạch Arduino R3**
Là dòng mạch thế hệ thứ 3 của Arduino UNO, dòng mạch được ưa chuộm khi dùng để lập trình. Hiện nay dòng mạch này cũng khá là phổ biến trên thị trường, giá cả rẻ hơn so với một số dòng mạch khác.

Arduino UNO có thể sử dụng 3 vi điều khiển họ 8bit AVR là ATmega8, ATmega168, ATmega328. Bộ não này có thể xử lí những tác vụ đơn giản như điều khiển đèn LED nhấp nháy, xử lí tín hiệu cho xe điều khiển từ xa, làm một trạm đo nhiệt độ - độ ẩm và hiển thị lên màn hình LCD.

Về phần năng lượng: Arduino UNO có thể được cấp nguồn 5V thông qua cổng USB hoặc cấp nguồn ngoài với điện áp khuyên dùng là 7-12V DC và giới hạn là 6-20V. Thường thì cấp nguồn bằng pin vuông 9V là hợp lí nhất nếu bạn không có sẵn nguồn từ cổng USB. Nếu cấp nguồn vượt quá ngưỡng giới hạn trên, bạn sẽ làm hỏng Arduino UNO.

Sau đây là một vài thông số của Arduno:

|Vi điều khiển|ATmega328 họ 8bit|
| :- | :- |
|Điện áp hoạt động|5V DC (chỉ được cấp qua cổng USB)|
|Tần số hoạt động|16 MHz|
|Dòng tiêu thụ|khoảng 30mA|
|Điện áp vào khuyên dùng|7-12V DC|
|Điện áp vào giới hạn|6-20V DC|
|Số chân Digital I/O|14 (6 chân hardware PWM)|
|Số chân Analog|6 (độ phân giải 10bit)|
|Dòng tối đa trên mỗi chân I/O|30 mA|
|Dòng ra tối đa (5V)|500 mA|
|Dòng ra tối đa (3.3V)|50 mA|
|Bộ nhớ flash|32 KB (ATmega328) với 0.5KB dùng bởi bootloader|
|SRAM|2 KB (ATmega328)|
|EEPROM|1 KB (ATmega328)|

1. #### ` `**<a name="_toc141495612"></a>Cảm biến nhiệt độ, độ ẩm DHT11**
Cảm biến độ ẩm và nhiệt độ DHT11 là cảm biến rất thông dụng hiện nay vì chi phí rẻ và rất dễ lấy dữ liệu thông qua chuẩn giao tiếp 1 wire. Chuẩn giao tiếp 1 wire là dùng 1 chân Digital để truyền dữ liệu.

Bộ tiền xử lý tín hiệu được tích hợp trong cảm biến giúp bạn có thể đọc dữ liệu chính xác mà không phải qua bất kỳ tính toán nào.

DHT11 gửi và nhận dữ liệu với một dây tín hiệu **DATA**, với chuẩn dữ liệu truyền 1 dây này, chúng ta phải đảm bảo sao cho ở chế độ chờ (idle) dây DATA có giá trị ở mức cao, nên trong mạch sử dụng DHT11, dây DATA phải được mắc với một trở kéo bên ngoài(thông thường giá trị là 4.7kΩ) , ở đây đã được tích hợp lại thành một module hoàn chỉnh.

`   `Thông số kĩ thuật của cảm biến:

- Điện áp hoạt động: 3V - 5V (DC)
- Dải độ ẩm hoạt động: 20% - 90% RH, sai số ±5%RH
- Dải nhiệt độ hoạt động: 0°C ~ 50°C, sai số ±2°C
- Khoảng cách truyển tối đa: 20m


1. #### <a name="_toc141495613"></a>**Còi chíp 5V** 
Còi chip 5V được dùng làm linh kiện trong các mạch điện tử tạo ra tín hiệu âm thanh.

Thông số kỹ thuật :

- Điện áp hoạt động: 3V – 5V
- Dòng hoạt động: <25mA
- Tần số âm thanh: 2500Hz

1. #### ` `**<a name="_toc141495614"></a>Led 5mm**
Đèn LED (Light-Emmiting Diode) hay còn gọi đi-ốt phát sáng là một linh kiện được sử dụng rất phổ biến bởi vì giá thành rẻ và cách sử dụng đơn giản. Đèn LED ngoài dạng thông thường - các đèn LED riêng lẻ, mỗi đèn phát sáng một màu xác định - còn có rất nhiều biến thể của nó. Có thể kể đến một số loại như: LED dây, LED 7 đoạn, ma trận LED,....

Thông số kỹ thuật:

- Điện áp hoạt động: 3.3 - 5V
- Dòng hoạt động: 20-30mA
- Kích thước: Đường kính 5mm

1. #### ` `**<a name="_toc141495615"></a>Màn hình LCD 1602**
Thiết bị hiển thị [**LCD 1602**](http://www.suachualaptop24h.com/Linh-kien-laptop.html) (Liquid Crystal Display) được sử dụng trong rất nhiều các ứng dụng của VĐK. LCD 1602 có rất nhiều ưu điểm so với các dạng hiển thị khác như: khả năng hiển thị kí tự đa dạng (chữ, số, kí tự đồ họa); dễ dàng đưa vào mạch ứng dụng theo nhiều giao thức giao tiếp khác nhau, tiêu tốn rất ít tài nguyên hệ thống, giá thành rẻ,…

Thông số kĩ thuật  của sản phẩm LCD 1602:

- Điện áp MAX : 7V
- Điện áp MIN : - 0,3V
- Hoạt động ổn định : 2.7-5.5V
- Điện áp ra mức cao : > 2.4
- Điện áp ra mức thấp : <0.4V
- Dòng điện cấp nguồn : 350uA - 600uA
- Nhiệt độ hoạt động : - 30 - 75 độ C

1. #### <a name="_toc141495616"></a>**Motor 3V**
`   `Motor giảm tốc mini 3V cũng tương tự như các loại motor giảm tốc thông thường, nhưng nó có kích thước và công suất nhỏ hơn nhằm phù hợp với các mục đích khác nhau.

`   `Motor giảm tốc mini 3V là một trong những loại motor giảm tốc có công suất thấp được thường xuyên sử dụng nhất.

`    `Do vậy, motor giảm tốc mini 3V có thể được sản xuất riêng để phù hợp cho từng loại công việc, từng thiết bị với ứng dụng khác nhau.

Thông số kĩ thuật:

- Điện áp hoạt động: 1 ~ 3VDC (điện áp càng cao tốc độ quay càng nhanh, lưu ý không vượt quá 3VDC sẽ làm cháy động cơ).
- Dòng điện tiêu thụ: 0.35~0.4A.
- Trục quay dài: 8mm.
- Đường kính trục quay: 2mm.
- Tốc độ quay tại 3VDC: 17.000~18.000PRM.
- Kích thước: 20 x 15 x 25mm.
  1. #### <a name="_toc141495617"></a>**Rơ-le 5VDC**
`     `Relay 5V là một công tắc chuyển đổi hoạt động bằng điện. Nói là một công tắc vì rơ le có 2 trạng thái ON và OFF. Relay ở trạng thái ON hay OFF phụ thuộc vào có dòng điện chạy qua relay hay không.

NGUYÊN TẮC HOẠT ĐỘNG:

`      `Khi có dòng điện chạy qua relay, dòng điện này sẽ chạy qua cuộn dây bên trong và tạo ra một từ trường hút. Từ trường hút này tác động lên một đòn bẩy bên trong làm đóng hoặc mở các tiếp điểm điện và như thế sẽ làm thay đổi trạng thái của relay. Số tiếp điểm điện bị thay đổi có thể là 1 hoặc nhiều, tùy vào thiết kế.
Relay có 2 mạch độc lập nhau họạt động. Một mạch là để điều khiển cuộn dây của relay: Cho dòng chạy qua cuộn dây hay không, hay có nghĩa là điều khiển relay ở trạng thái ON hay OFF. Một mạch điều khiển dòng điện ta cần kiểm soát có qua được relay hay không dựa vào trạng thái ON hay OFF của relay.

Thông số kĩ thuật:

- Tên mã: SRD-05DVC-SL-C
- Phân loại: Relay cơ
- Điện áp điều khiển: 5VDC
- Dòng điện tối đa qua tiếp điểm: 10A
- Số chân: 5 chân
- Số kênh: 1 kênh 













1. # <a name="_toc79515940"></a><a name="_toc141495618"></a>**NGUYÊN LÍ HOẠT ĐỘNG CỦA HỆ THỐNG**
Buồng ấp trứng (hay còn gọi là máy ấp trứng) là một thiết bị được sử dụng để tạo điều kiện lý tưởng để trứng gia cầm phát triển và nở thành con. Nguyên lí hoạt động của buồng ấp trứng xoay quanh việc duy trì môi trường ấm, ẩm, và giữ cho trứng ở vị trí nằm ngang để giúp trứng phát triển thành con một cách thành công. Dưới đây là một số yếu tố chính của nguyên lí hoạt động của buồng ấp trứng:

1\. Nhiệt độ: Máy ấp trứng duy trì nhiệt độ ấm cúng, xấp xỉ nhiệt độ tự nhiên mà mẹ gà sẽ tạo ra bằng cách ấp trứng bằng cơ thể mình. Nhiệt độ phổ biến cho trứng gia cầm thường là từ 37,5°C đến 38,5°C.

2\. Độ ẩm: Độ ẩm là yếu tố quan trọng để đảm bảo sự thành công trong việc ấp trứng. Môi trường ẩm ướt giúp duy trì độ ẩm tự nhiên trong trứng, giúp lớp màng bọc bên ngoài không bị khô và dẻo dai, từ đó giúp con gà con dễ dàng gãi để thoát ra khỏi vỏ trứng.

3\. Quản lý giả lập yếu tố tự nhiên: Máy ấp trứng được điều khiển bằng cách giả lập các yếu tố tự nhiên như nhiệt độ và độ ẩm để đảm bảo môi trường lý tưởng cho sự phát triển của trứng.

4\. Đèn hồng ngoại: Một số buồng ấp trứng có đèn hồng ngoại để tạo ra nhiệt từ xa, giúp duy trì nhiệt độ ổn định trong buồng.

Nhờ vào các nguyên lý hoạt động này, buồng ấp trứng tạo ra môi trường lý tưởng để tăng khả năng nở của trứng gia cầm và cải thiện tỷ lệ nở thành công. Các thông số cụ thể về nhiệt độ, độ ẩm và thời gian ấp trứng sẽ phụ thuộc vào loại gia cầm và loại trứng cụ thể mà bạn đang ấp.

Quy trình hoạt động của buồng ấp trứng cũng là một quy trình khép kín. Khi có hiện tượng về sự phát triển của gà con (như nhiệt độ gia tăng đột ngột, sự tăng trưởng của trứng), các cảm biến và thiết bị đầu vào (cảm biến nhiệt độ, cảm biến độ ẩm) trong buồng ấp trứng sẽ nhận tín hiệu và truyền thông tin về trạng thái của trứng về trung tâm điều khiển của buồng ấp.

Tại trung tâm điều khiển, các thông tin nhận được sẽ được xử lý để đảm bảo môi trường trong buồng ấp trứng đạt điều kiện lý tưởng cho sự phát triển của gà con. Nếu cần thiết, hệ thống sẽ điều chỉnh nhiệt độ, độ ẩm và các yếu tố khác để đảm bảo trứng được ấp nở một cách thành công và đồng đều.

Các thiết bị đầu ra (màn hình hiển thị, đèn báo, còi báo) trong buồng ấp trứng sẽ nhận tín hiệu từ trung tâm điều khiển và thực hiện các chức năng tương ứng. Ví dụ, khi gà con chuẩn bị nở, hệ thống có thể kích hoạt đèn báo hoặc âm thanh để thông báo cho người quản lý hoặc nhân viên biết rằng có gà con sắp nở.






















1. # <a name="_toc79515941"></a><a name="_toc141495619"></a>**LẬP TRÌNH CHO HỆ THỐNG**
   1. ## <a name="_toc141495620"></a>**Code Qt**







- File Code Qt và Arduino được lưu trữ tại đây: [tuanpd205459/buong-ap-trung (github.com)](https://github.com/tuanpd205459/buong-ap-trung)
- Chương trình có sử dụng thư viện QcustomPlot để tạo biểu đồ nhiệt theo thời gian thực.
  - Thư viện QcustomPlot: : [Qt Plotting Widget QCustomPlot - Download](https://www.qcustomplot.com/index.php/download)




1. # <a name="_toc79515942"></a><a name="_toc141495622"></a>**KẾT LUẬN**
Qua việc nghiên cứu đề tài, nhóm chúng em đã tự rút ra được những ưu điểm cũng như nhược điểm của bản thân như về ưu điểm, chúng em đã vận dụng được những kiến thức đã được học để chế tạo thành công một buồng ấp trứng tự động và đo độ ẩm sử dụng cảm biến DHT11 với những linh kiện rẻ, dễ mua. Đồng thời cũng có thể thành công giao tiếp với hệ thống thông qua giao diện Qt trên máy tính. Về nhược điểm, bọn em chưa kết nối được wifi, bluetooth cũng như phát triển ứng dụng trên điện thoại để phù hợp hơn với mục tiêu ban đầu đề ra là phát triển sản phẩm theo hướng gia dụng, dễ sử dụng, phù hợp quy mô hộ gia đình. 

` `Về hướng phát triển trong tương lai, với việc những công nghệ hiện đại được áp dụng vào lĩnh vực Nông nghiệp ngày càng nhiều. Vì vậy, nhóm chúng em sẽ nghiên cứu để tích hợp modun wifi và bluetooth vào hệ thống, đồng thời tạo app trên điện thoại để dễ dàng sử dụng, thân thiện hơn với người dùng đúng như mục tiêu ban đầu của nhóm. Nhóm cũng sẽ tạo vỏ bọc bằng vật liệu, tăng tính thẩm mĩ cho sản phẩm, giúp dễ dàng lắp đặt trong nhà riêng và để bảo vệ hệ thống. 
















# <a name="_toc79515943"></a><a name="_toc141495623"></a>**DANH MỤC HÌNH VẼ**
![Hình 1. Mạch Arduino Uno R3	3](http://mlab.vn/image/data/Bai%20viet%20ky%20thuat/Arduino/bai%202%20Nhung%20dieu%20co%20ban/ArduinoUnoR3.jpg))

![Hình 2. Cảm biến DHT11	3](https://nshopvn.com/wp-content/uploads/2019/03/module-cam-bien-do-am-nhiet-do-dht11-UIJ3-2020-600x600.jpg)

![Hình 3. Màn hình LCD 1602	3](https://nshopvn.com/wp-content/uploads/2019/03/man-hinh-lcd-1602-xanh-la-67HZ-2020-600x600.jpg)

![Hình 4. Rơ-le 5VDC	3](https://chipviet.vn/wp-content/uploads/2019/12/1_MACH-DEM-RELAY-1-KENH-5V-10A.jpg)

![Hình 5. Còi Buzzer báo động	5](https://nshopvn.com/wp-content/uploads/2019/03/coi-buzz-thu-dong-5v-xsqe-1-600x600.jpg)

![Hình 6. Motor 3V	5](https://bizweb.dktcdn.net/thumb/1024x1024/100/251/524/products/z2660621500628-c46040939cef504e82d0d3ec1267cb89-df2cc177-2d66-41ae-9395-b48cab9009bb.jpg?v=1676456976013)

![Hình 7. Đèn Led 5mm	5](https://bizweb.dktcdn.net/100/408/906/products/led-7-mau-2-chan-3vdc-5mm-1-7d832712-a976-4e81-b316-5aaaf99789bf.jpg?v=1616560389000)

[Hình 8. Giao diện Qt trên máy tính	9](https://github.com/tuanpd205459/buong-ap-trung/blob/main/giao-dien-qt.png)


# <a name="_toc141495624"></a>**DANH MỤC BẢNG BIỂU**
[Bảng 1. Sơ đồ đấu nối	4](https://github.com/tuanpd205459/buong-ap-trung/blob/main/Proteus/so-do-mach.png)


