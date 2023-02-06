/*==========================================================================
 *
 *  Copyright 2011 あるふぁ～秘密基地(Alpha Secret Base) & DONG. All Rights Reserved.
 *
 *  ○ゲームタイトル：WispLisp Array of List
 *  ○ジャンル      ：シューティング
 *  ○プレイ人数    ：1人
 *  ○バージョン    ：1.01
 *  ○公開日付      ：2011/07/30
 *  ○更新日付      ：2011/07/31
 ==========================================================================*/

・はじめに
  今、確かなクソゲーが必要です。
  高速弾をかわした所を物量で撃ちぬいてシューターを皆殺しにして楽しむゲームです。俺が。
  火力にはマンダラを。マンダラには物量を。新生WispLisp、誕生。

  Windows版はfullscreen.batを実行するとフルスクリーンで起動します。
  実行ファイルの引数オプション
    -f フルスクリーン

  各プラットホームでビルドする事が可能です。
  act.exe :Windows用の実行ファイル

  Windows版をコンパイルするにあたって
    makefileはMakefile.winです。

  以下のファイルは実際に検証してませんので、参考にしてもらればと思います。
  MacOSX版をコンパイルするにあたって
    makefileはMakefile.macosxです。

  Linux版をコンパイルするにあたって
    makefileはMakefile.linuxです。


  本プログラムはSDL(Simple Directmedia Layer)を使って作成しています
  SDLはLGPLに従って配布されています
  http://www.libsdl.org/license.php

  SDL(Simple Directmedia Layer)
  http://www.libsdl.org/

・あらすじ
  まるで空っぽの器のような、躰をやっとのことで起こす。
  蛍光灯で照らしたように、青白い世界。
  軽すぎる体。
  私は本当にここにいるのだろうか。
  あるいは、この体は、この目は、私のものではないようにも思えた。

  …けだるい…

  私は、目の前の羊皮紙を見た。
  ずっと前からそこにあったかのように
  何かのリストがはためいていた。

  意味不明な文字で書かれたリストが、
  この世界での私の役割を物語っているのだろう。

・操作方法
  上下：項目の選択
  左右：キャラクターの移動、項目の選択
  パッド1（キーボード:Z）    ：ショット（オート連射）
  パッド2（キーボード:X）    ：マンダラ
  パッド3（キーボード:C）    ：ショット（長押しで低速）
  パッド4（キーボード:V）    ：ポーズ
  パッド7（キーボード:F1）   ：音量を小さくする
  パッド8（キーボード:F2）   ：音量を大きくする
  キーボード:ESC             ：強制終了

・メニュー
  GameStart  ：ゲームを開始します。
  Exit       ：アプリケーションを終了します。

・システム
  マンダラで周囲の敵弾を吸収して強力な攻撃を行います。
  使用後はマンダラが回復するまで使用できません。
  接近してショットで攻撃するとマンダラアイテムが出現します。
  取得すると、マンダラの回復が早くなります。

  黒羊を倒すと次の敵編隊が出現します。
  ショットで敵を倒すとREDがあがり、被弾するとREDが下がります。
  REDが高いと敵が多くなり難しくなります。（ボスも含む）
  
  マンダラで敵を倒すとスコアアイテムが出現します。
  RED高いほどスコアアイテムの点数も増えます。

  ミスをすると残り時間がへります。
  中ボス、ボスを倒すと残り時間が少し増えます。
  REDが高い状態でボスをたくさん出現させれば、多くの時間を回復できるかもしれません。

・スタッフ
  D.K
  丼
  おめが
  TERU


・御意見、ご感想などはこちらまで。
    dk@red.interq.or.jp
    http://maglog.jp/alpha-secret-base/


・更新履歴
  2011/01/25    Ver0.01
                つくりかけ
  2011/07/30    Ver1.00
                正式リリース
  2011/07/31    Ver1.01
                EASYの難易度を下げました。


・ライセンス
やわらかライセンス のもと配布されます。

<JAPANESE>
ライセンス
-------

Copyright 2011 あるふぁ～秘密基地(Alpha Secret Base) & DONG. All Rights Reserved.

  免責・著作・配布 
  柔かいコト 
    本ゲームは「みんなで楽しく」やわらかいです。 

    本ゲームを改造したりイジったバージョンを配布しても、何らOKです。改造たのしーい！ 
    改造したところや改造した人の名前を、分かりやすいところに書いておくと 
    いいかもしれません。 

    ナイスな改造が出来たら　メールなどで教えて。俺にも遊ばせてください。 

    改造とか一切してないものは好きにコピーして配布しちゃって結構結構。 


<ENGLISH>
License
-------

Copyright 2011  Alpha Secret Base. & DONG. All rights reserved.

Disclaimer / Copyright / Redistribution 

  This game is under the "Yawaraka(flexible)" license. 

    Use at your own risk. 
    You can freely modify and redistribute it. Conversions are really Fun! 
    You should write the changes and the person who did them in the 
    readme file or in a place that was obvious to find. 

    If you make a cool modification, please tell me via mail or by any 
    other means, even though that's not a condition of the license. 
    Let me enjoy your changes too. 

    If you have not done any modifications, you can freely distribute 
    without problems.


