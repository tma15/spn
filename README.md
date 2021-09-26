# SPN
A C++ implementation of SPIN model for extracting semantic orientations for Japanese.

To implement this program, I referred the following paper:

> Extracting Semantic Orientations of Words using Spin Model
>
> Hiroya Takamura, Takashi Inui and Manabu Okumura
>
> ACL 2005
>
> https://aclanthology.org/P05-1017.pdf

## Reqruirements
- MeCab
- sqlite3


## Install
```
mkdir build
cd build
cmake -DCMAKE_INSTALL_PREFIX=/path/to/install -DMECAB_PREFIX=`mecab-config --prefix` ..
make
make install
```

## Usage
To execute this program, you must download Japanese WordNet.
```
wget http://compling.hss.ntu.edu.sg/wnja/data/1.1/wnjpn.db.gz
tar zxvf wnjpn.db.gz
```


After that, you can execute this program.

```
spn_build --wordnet ~/data/wnjpn.db
```


Finally, you will obtain results in `out` file.
A score in the first column denotes the porality score of the word.
You can regard words those scores are larger than 0 as positive words vice versa.


```
-1 ひどい
-1 悪い
-0.0406709 人込み
-0.0390435 ずたずた
-0.0390435 切裂く
-0.0329183 下回る
-0.0317677 せせくる
-0.0315665 困憊
-0.0315127 疲らす
-0.0303288 えがらっぽい
-0.0275977 とんでも
-0.0275938 狂わす
-0.0273271 クラッジ
-0.02725 おぞましい
...
0.0316195 たのもしい
0.0316195 多望
0.0316195 末頼もしい
0.0316195 洋々
0.0316195 洋洋
0.031778 萌える
0.0318674 ひらめく
0.0328165 前途
0.0328165 順境
0.0335482 ベネチアングラス
0.0339151 変幻
0.0365089 とりこ
0.0375192 篤行
0.0380027 有望
0.0459215 スリリング
1 素晴らしい
1 良い
```
