# Adventure Time




## Giới thiệu
- Authors : Quách Đức Thiện (23020161) & Cao Trần Hà Thái(23020152)
- K68 C-C UET , VNU


"Adventure Time" kể về câu chuyện về một thủy thủ tài ba mang tên Jack, người đã bị cuốn vào một cuộc phiêu lưu đơn độc khi tìm kiếm một kho báu huyền thoại trên hòn đảo hoang.

Jack, người luôn khao khát khám phá và phiêu lưu, đã nghe đồn về một kho báu vĩ đại được gọi là "Hải Tặc Vàng", được cho là giấu kín trên một hòn đảo hoang bí ẩn. Theo huyền thoại, kho báu này chứa đựng một lượng khổng lồ vàng, bạc và các vật phẩm quý giá khác, đủ để làm giàu bất cứ ai tìm ra nó.

Dẫn dắt bởi sự tò mò và lòng dũng cảm, Jack lên đường khám phá một mình. Nhưng trên con đường tìm kiếm kho báu, anh phải đối mặt với vô vàn khó khăn và nguy hiểm. Hòn đảo hoang không chỉ là nơi sinh sống của những sinh vật kỳ quái và thực vật độc hại mà còn là nơi trú ngụ của những kẻ thù lớn mạnh và tìm cách ngăn chặn bất cứ ai tiến gần đến kho báu.

Bằng sự thông minh, sức mạnh và lòng can đảm, Jack phải vượt qua mọi thử thách để tiếp tục cuộc hành trình. Tuy nhiên, không chỉ là vật lý, mà còn là tinh thần. Trên con đường dẫn tới kho báu, anh phải đối diện với những bí ẩn, những bí mật của quá khứ và những quyết định khó khăn về tình bạn, lòng tin và trách nhiệm.

Tuy nhiên, mối nguy lớn nhất không chỉ đến từ những sinh vật hung dữ mà còn từ bản thân sự tồn tại của Jack. Anh phải đối mặt với những thử thách tâm lý, nhớ về quá khứ và tìm ra lối thoát cho mình. Liệu anh có thể vượt qua được mọi khó khăn, trỗi dậy từ đống tro tàn và trở về với thế giới bên ngoài?
## Hướng dẫn cài đặt 
- Bước 1 : Bấm vào "Dowload zip" ở phía dưới của code để tải về thư mục nén game.
- Bước 2 : Giải nén file zip vừa được tải về.
- Bước 3 : Dùng 1 complie đã cài đặt trình biên dịch C++ và SDL2 sau đó dùng terminal complie file main.cpp trong project bằng lệnh sau :
g++ -c src/main.cpp -std=c++14 -m64 -g -Wall -I include && g++ main.o -o bin/debug/main -lSDL2 -lSDL2_image -lSDL2_mixer -lSDL2_ttf && ./bin/debug/main
- Bước 4 : Gác chân lên và thoải mái đắm chìm vào chuyến phiêu lưu cùng Jack trong tựa game.
## Nội dung của game
Bạn sẽ dành chiền thắng khi tiêu diệt vượt qua hết các thử thách , tiêu diệt hết các quái vật và tìm được kho báu trên đảo
### Hướng dẫn chơi

- A : đi sang bên trái
- D : đi sang bên phải
- Space : Nhảy lên
- J : Attack 1
- K : Attack 2
- L : Attack 3
### Các đối tượng có trong game

#### Vật phẩm
- Thuyền : Chiếc thuyền giúp Jack di chuyển quanh hòn đảo , khi nhảy lên nó sẽ căng buồm và đưa bạn tới đích.

![1 gif](https://github.com/thienquach33/Adventure-Time/blob/main/assets/pic/ship.gif)
- Chìa khóa & Kho báu : Bạn cần tìm được chìa khóa để có thể mở được hòm kho báu.

![2 gif](https://github.com/thienquach33/Advanture-Time-/blob/main/assets/pic/chest.gif)

- Tiền : Có 2 loại tiền là tiền vàng và tiền bạc , khi nhặt được mỗi loại tiền sẽ cho bạn tăng 1 số điểm nhất định.

![3 gif](https://github.com/thienquach33/Advanture-Time-/blob/main/assets/pic/coin.gif)

- Đá quý : Có 3 loại đá quý , khi nhặt được mỗi loại đá sẽ cho bạn tăng 1 số điểm nhất định.

![4 gif](https://github.com/thienquach33/Advanture-Time-/blob/main/assets/pic/diamod.gif)

- Thuốc tăng sức mạnh : Uống vào sẽ được thể lực bền bỉ , tăng sức bật , sức chạy.

![5 gif](https://github.com/thienquach33/Adventure-Time/blob/main/assets/pic/postion.gif)

#### Quái vật
- Cá Răng Sắc (Sharpjaw): Loài cá kích thước lớn với hàm răng sắc nhọn như lưỡi dao. Chúng sống trong vùng biển xung quanh hòn đảo và rình rập các thủy thủ bất cẩn bằng cách tấn công bất ngờ từ dưới đáy biển.

![6 gif](https://github.com/thienquach33/Advanture-Time-/blob/main/assets/pic/fish.gif)

- Vua Cua Đỏ (Redclaw King): Một con cua khổng lồ với chiều rộng vượt qua mười mét, được biết đến với sự hung dữ và khí phách. Vua Cua Đỏ là bậc thầy của vùng biển này, và nó sẵn lòng bảo vệ lãnh thổ của mình bằng bất kỳ giá nào.

![7 gif](https://github.com/thienquach33/Advanture-Time-/blob/main/assets/pic/king_crab.gif)

- Sao Biển Ám (Shadow Starfish): Loài sao biển đen tối, có khả năng phát sáng rất yếu, giúp chúng lẩn trốn trong bóng tối của lòng đại dương. Tuy nhiên, khả năng này cũng làm cho chúng trở nên đáng sợ khi chúng tấn công bất ngờ từ bên dưới và hút sức mạnh từ nạn nhân của chúng.

![8 gif](https://github.com/thienquach33/Advanture-Time-/blob/main/assets/pic/star.gif)

#### Chướng ngại vật

- Trụ thổ dân : Những thổ dân ở vùng đất này vô cùng hung dữ nhưng họ lại quá bận rộn để có thể theo dõi và canh chừng hòn đảo. Vì vậy , họ dựng lên 1 pháo đài vô cùng kiên cố và nguy hiểm cho bất cứ ai có ý định lăm le đến vùng đất của họ.

![9 gif](https://github.com/thienquach33/Advanture-Time-/blob/main/assets/pic/totem.gif)

- Bom nguyên tử : Khi đi vào vùng bom nổ sẽ khiến bạn bị nổ tan xác!

![10 gif](https://github.com/thienquach33/Adventure-Time/blob/main/assets/pic/bomb.gif)
