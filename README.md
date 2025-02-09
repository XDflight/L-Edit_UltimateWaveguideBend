# L-Edit Ultimate Waveguide Bend 波导弯曲绘制

> This project is documented in both **English & Chinese**. / 本项目文档为**中英双语**。

A ***Macro*** for drawing **High-Performance Waveguide Bend** in ***L-Edit*** :tm:. / 一个用于在 ***L-Edit*** :tm: 中绘制**高性能波导弯曲**的***插件宏（Macro）***。

## Install 安装方法
Follow these steps to install this Macro: / 按照以下步骤安装此插件宏：
1. [Download the DLL / 下载 DLL](https://github.com/XDflight/L-Edit_UltimateWaveguideBend/releases)
1. Open L-Edit :tm:, add the DLL to "Tools -> Macro" / 打开 L-Edit :tm:，在“Tools -> Macro”中添加 DLL。
1. You may find this Macro at "Draw -> Macros" / 可在 “Draw -> Macros” 中找到此插件宏
1. You may also use hotkey "Ctrl-B" to run this Macro (recommended) / 也可使用快捷键 “Ctrl+B” 执行此插件宏（建议）
1. Run the Macro, and follow the instructions on the window. / 执行插件宏，根据窗口提示进行操作。

*Note: You have to add the DLL every time you open L-Edit :tm:. / 注意：每次启动 L-Edit :tm: 都需重新添加DLL。*

## Usage 使用方法
Follow these steps to use this Macro: / 按照以下步骤使用此插件宏：
1. Run the Macro / 运行插件宏
1. First, choose an arbitary point on the 1st wire (except the connecting point). / 首先，选择第1条线段上任意一点（不要是连接端点）。
1. Then, click on the connecting point of the 1st wire. / 之后，点击第1条线段的连接端点。
1. Next, click on the connecting point of the 2nd wire. / 然后，点击第2条线段的连接端点。
1. Choose an arbitary point on the 2nd wire (except the connecting point). /选择第2条线段上任意一点（不要是连接端点）。
1. Input the precision of the curve you want to generate in terms of the total number of interpolated points (default: `100`). / 输入要生成的曲线的精度（根据插值点总数；默认：`100`）。
1. If the input is correct, the curve will be automatically generated; otherwise, a corresponding error will be prompted. / 如果输入正确，程序则会自动生成波导弯曲；否则，会提示相应错误。
1. Once you are familiar with these operations, you can draw waveguide bends very efficiently. / 熟悉操作之后，可以非常高效的进行波导弯曲绘制。
1. In most cases, as long as the points on the two wires do not overlap with the connecting points, this Macro should be able to generate a connection curve without the geometry of the wires meeting any requirements; 
   however, due to unavoidable numerical stability issues, some connections with excessive bending angles cannot be satisfactorily satisfied, and the program will remind you when if that's the case (you can choose whether or not to continue drawing the curve; curve quality is not guaranteed). 
   It should be noted that if a connection cannot be satisfied by the Macro, usually no other connection method can be found. 
   / 绝大多数情况下，只要两个线段上的点不与连接端点重合，程序均能够生成连接曲线，无需几何形状满足任何要求；
   不过，由于无法避免的数值稳定性问题，存在部分弯曲角度过大的连接无法被良好满足，程序在相应情况下会予以提醒（可选择是否继续绘制；不保证绘制质量）。
   需要注意的是，如果一个连接无法被程序满足，通常也找不到其他连接方式。

## Build 构建方法
If you want to build the DLL yourself, proceed to the following steps: 
/ 如需要自行构建 DLL，可进行以下操作：
1. Install Visual Studio 2010 (for compiler) / 安装 Visual Studio 2010 开发环境（为了编译器）
1. Clone the repository / 克隆仓库
   ```
   git clone https://github.com/XDflight/L-Edit_UltimateWaveguideBend.git
   ```
1. Open with Visual Studio 2022 / 使用 Visual Studio 2022 打开
1. Build solution / 生成解决方案
1. DLL should be at `./<arch x64/x86>/<config Debug/Release>` / DLL 应位于 `./<架构 x64/x86>/<配置 Debug/Release>`

## Note 注意事项
 - Only tested for L-Edit :tm: `v16.30` / 只对 L-Edit :tm: `v16.30` 进行了测试

## Copying 外引项目
 - [Eigen](https://gitlab.com/libeigen/eigen.git) (for linear algebra / 用于线性代数)
 - L-Edit :tm: UPI Link 

## License 开源协议
See [LICENSE](LICENSE.txt). / 见 [LICENSE](LICENSE.txt).
