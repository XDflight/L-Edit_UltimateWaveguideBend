# L-Edit Ultimate Waveguide Bend ������������

> This project is documented in both **English & Chinese**. / ����Ŀ�ĵ�Ϊ**��Ӣ˫��**��

A ***Macro*** for drawing **High-Performance Waveguide Bend** in ***L-Edit*** :tm:. / һ�������� ***L-Edit*** :tm: �л���**�����ܲ�������**��***����꣨Macro��***��

## Install ��װ����
Follow these steps to install this Macro: / �������²��谲װ�˲���꣺
1. [Download the DLL / ���� DLL](https://github.com/XDflight/L-Edit_UltimateWaveguideBend/releases)
1. Open L-Edit :tm:, add the DLL to "Tools -> Macro" / �� L-Edit :tm:���ڡ�Tools -> Macro������� DLL��
1. You may find this Macro at "Draw -> Macros" / ���� ��Draw -> Macros�� ���ҵ��˲����
1. You may also use hotkey "Ctrl-B" to run this Macro (recommended) / Ҳ��ʹ�ÿ�ݼ� ��Ctrl+B�� ִ�д˲���꣨���飩
1. Run the Macro, and follow the instructions on the window. / ִ�в���꣬���ݴ�����ʾ���в�����

*Note: You have to add the DLL every time you open L-Edit :tm:. / ע�⣺ÿ������ L-Edit :tm: �����������DLL��*

## Usage ʹ�÷���
Follow these steps to use this Macro: / �������²���ʹ�ô˲���꣺
1. Run the Macro / ���в����
1. First, choose an arbitary point on the 1st wire (except the connecting point). / ���ȣ�ѡ���1���߶�������һ�㣨��Ҫ�����Ӷ˵㣩��
1. Then, click on the connecting point of the 1st wire. / ֮�󣬵����1���߶ε����Ӷ˵㡣
1. Next, click on the connecting point of the 2nd wire. / Ȼ�󣬵����2���߶ε����Ӷ˵㡣
1. Choose an arbitary point on the 2nd wire (except the connecting point). /ѡ���2���߶�������һ�㣨��Ҫ�����Ӷ˵㣩��
1. Input the precision of the curve you want to generate in terms of the total number of interpolated points (default: `100`). / ����Ҫ���ɵ����ߵľ��ȣ����ݲ�ֵ��������Ĭ�ϣ�`100`����
1. If the input is correct, the curve will be automatically generated; otherwise, a corresponding error will be prompted. / ���������ȷ����������Զ����ɲ������������򣬻���ʾ��Ӧ����
1. Once you are familiar with these operations, you can draw waveguide bends very efficiently. / ��Ϥ����֮�󣬿��Էǳ���Ч�Ľ��в����������ơ�
1. In most cases, as long as the points on the two wires do not overlap with the connecting points, this Macro should be able to generate a connection curve without the geometry of the wires meeting any requirements; 
   however, due to unavoidable numerical stability issues, some connections with excessive bending angles cannot be satisfactorily satisfied, and the program will remind you when if that's the case (you can choose whether or not to continue drawing the curve; curve quality is not guaranteed). 
   It should be noted that if a connection cannot be satisfied by the Macro, usually no other connection method can be found. 
   / �����������£�ֻҪ�����߶��ϵĵ㲻�����Ӷ˵��غϣ�������ܹ������������ߣ����輸����״�����κ�Ҫ��
   �����������޷��������ֵ�ȶ������⣬���ڲ��������Ƕȹ���������޷����������㣬��������Ӧ����»��������ѣ���ѡ���Ƿ�������ƣ�����֤������������
   ��Ҫע����ǣ����һ�������޷����������㣬ͨ��Ҳ�Ҳ����������ӷ�ʽ��

## Build ��������
If you want to build the DLL yourself, proceed to the following steps: 
/ ����Ҫ���й��� DLL���ɽ������²�����
1. Install Visual Studio 2010 (for compiler) / ��װ Visual Studio 2010 ����������Ϊ�˱�������
1. Clone the repository / ��¡�ֿ�
   ```
   git clone https://github.com/XDflight/L-Edit_UltimateWaveguideBend.git
   ```
1. Open with Visual Studio 2022 / ʹ�� Visual Studio 2022 ��
1. Build solution / ���ɽ������
1. DLL should be at `./<arch x64/x86>/<config Debug/Release>` / DLL Ӧλ�� `./<�ܹ� x64/x86>/<���� Debug/Release>`

## Note ע������
 - Only tested for L-Edit :tm: `v16.30` / ֻ�� L-Edit :tm: `v16.30` �����˲���

## Copying ������Ŀ
 - [Eigen](https://gitlab.com/libeigen/eigen.git) (for linear algebra / �������Դ���)
 - L-Edit :tm: UPI Link 

## License ��ԴЭ��
See [LICENSE](LICENSE.txt). / �� [LICENSE](LICENSE.txt).
