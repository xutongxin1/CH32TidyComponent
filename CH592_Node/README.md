# 这是一个CH592的工程模板

如何使用：

1. 确保安装好了原厂的MRS，确保你的单片机原厂可以下载

   ![image-20241214180835173](http://github.xutongxin.me/https://raw.githubusercontent.com/xutongxin1/PictureBed/master/2024After/image-20241214180835173.png)

2. 解压CH592Lib.zip到该文件夹的上一级，这样可以使你的多个项目都共同引用这些简单的依赖

   ![image-20241214180726640](http://github.xutongxin.me/https://raw.githubusercontent.com/xutongxin1/PictureBed/master/2024After/image-20241214180726640.png)

   （注：CH592库不会自己更新，目前是2024.12.14的最新版但不保证未来是否会更新，如果官网更新请自行下载资源包进行替换更新）

3. 打开CLion加载cmake，和stm32一样，CLion中所选择的工具链不重要，你需要在环境位置增加环境变量

```
Path=C:\Path\MounRiver\MounRiver_Studio\toolchain\RISC-V Embedded GCC\bin\
```

4. 添加或修改openocd的文件位置

   ![image-20241214203058256](http://github.xutongxin.me/https://raw.githubusercontent.com/xutongxin1/PictureBed/master/2024After/image-20241214203058256.png)

5. 添加openocd下载并运行配置

   ![image-20241214221636911](http://github.xutongxin.me/https://raw.githubusercontent.com/xutongxin1/PictureBed/master/2024After/image-20241214221636911.png)

   要修改source下的文件路径

   ![image-20241214221904285](http://github.xutongxin.me/https://raw.githubusercontent.com/xutongxin1/PictureBed/master/2024After/image-20241214221904285.png)

   其中source请直接引用官方MRS下，openocd的**bin**下的文件，别搞错，因为同名很多，文件应该大概长这样

   ![image-20241214200056012](http://github.xutongxin.me/https://raw.githubusercontent.com/xutongxin1/PictureBed/master/2024After/image-20241214200056012.png)

   ![image-20241214200249142](http://github.xutongxin.me/https://raw.githubusercontent.com/xutongxin1/PictureBed/master/2024After/image-20241214200249142.png)

6. 添加一次.gdbinit到你的用户文件夹下，使得模板下gdbinit的可以有效加载

   ![image-20241214203010741](http://github.xutongxin.me/https://raw.githubusercontent.com/xutongxin1/PictureBed/master/2024After/image-20241214203010741.png)

7. Enjoy！



## Q&A：

### Q:都是592，592X和592F编译环境有区别吗？

A:似乎没有，我对比过在修改芯片型号后官方MRS的项目文件没有一点变化除了一个信息文件，考虑到库也一样可能确实就是没区别吧

但是还是建议打开模板中的MRS修改芯片型号后再继续回到CLion编程

模板默认是592F



### Q:Error: The specified debug interface was not found (wlinke)

A:没选正确的openocd路径