# ResistorCheckerBase

<img src="https://github.com/akita11/ResistorChecker/blob/main/ResistorCheckerBase.jpg" width="240px">

<img src="https://github.com/akita11/ResistorChecker/blob/main/ResistorCheckerBase_w_ATOM.jpg" width="240px">

<img src="https://github.com/akita11/ResistorChecker/blob/main/ResistorCheckerBase_w_ATOM2.jpg" width="240px">

M5Stack ATOMシリーズを接続し、端子間の抵抗を計測できます。抵抗値が所定の範囲に収まっているかのチェックなどに使えます。抵抗測定レンジはジャンパピンで4段階に設定できます。また小型の圧電ブザーも搭載しています。


## 使い方

FW/内のプログラムを参考に、抵抗測定値に応じた動作を記述し、それをATOMシリーズに書き込んで本機に接続し、使用します。
例としてFW/には、抵抗測定値が抵抗測定レンジ（ジャンパピンで選択した参照抵抗値）の±10%以内に収まっていれば緑点灯（440Hzの音）、それ以下であれば赤点灯（880Hzの音）、それ以上であれば紫点灯（220Hzの音）となる動作を記述してあります。


## 抵抗測定レンジの設定

<img src="https://github.com/akita11/ResistorChecker/blob/main/ResistorCheckerBase_setting.jpg" width="240px">

測定対象の抵抗値にあわせて、抵抗測定レンジを設定することで測定精度を高めることができます。

- レンジ=L側 & 値=100側 : 100Ω
- レンジ=L側 & 値=1k側 : 1kΩ
- レンジ=H側 & 値=10k側 : 10kΩ
- レンジ=H側 & 値=100k側 : 100kΩ


## 技術的情報（測定原理）

抵抗値の測定は、本機の参照抵抗（100/1k/10k/100kからジャンパピンで選択）との分圧をATOMのアナログ入力ピンで計測しています。

測定対象の抵抗値をR、本機の参照抵抗値をRrefとすると、ATOMで測定される分圧Vmeasureは以下の式で求められます(VDD=3.3V)。この四季を使って、VmeasureからRを求めることができます。

Vmeasure = (Rref / (R + Rref) * VDD 



## Author

Junichi Akita (@akita11) / akita@ifdl.jp
