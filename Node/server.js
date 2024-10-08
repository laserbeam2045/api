const bodyParser = require("body-parser")
const express = require("express")
const app = express()

const DROP_TYPE_MAX = 10
const NONE = 0
const FIRE = 1
const WATER = 2
const WOOD = 3
const LIGHT = 4
const DARK = 5
const HEART = 6
const BLOCK = 7
const POISON = 8
const DEADLY_POISON = 9
const BOMB = 10

const ANUBIS = 0
const METATRON = 1
const KOMASAN = 2
const AMEN = 3
const HYLEN = 4
const COCO = 5
const VEROAH = 6
const YASHAMARU = 7
const SHIKI = 8
const TRAGON = 9
const APOLLON = 10
const AMAKOZUMI = 11
const WRATH = 12
const KYLO_REN = 13
const ALLATU = 14
const SUKUNA = 15

app.use(bodyParser.urlencoded({extended: false}))
app.use(bodyParser.json())
app.use(function(req, res, next){
  res.header("Access-Control-Allow-Origin", "*")
  next()
})


// 盤面で可能な最大コンボ数と、最大倍率を返すAPI
app.get('/api/maxData/:leader1-:leader2-:board', function(req, res){
  const leader1 = Number(req.params.leader1),
        leader2 = Number(req.params.leader2),
        board = JSON.parse(req.params.board)

  const boardData = getBoardData(board)

  addCombo(boardData, leader1, true)
  addCombo(boardData, leader2, false)
  multiplyMagnification(boardData, leader1)
  multiplyMagnification(boardData, leader2)

  res.send(JSON.stringify(boardData))
}).listen(1200)

console.log("Node server listening...")


// コンボ加算系リーダースキルを適用する関数
function addCombo(boardData, leader, firstTime) {
  switch (leader) {
  case HYLEN:
    if (
      7 <= boardData['dropCountArray'][FIRE] &&
      7 <= boardData['dropCountArray'][WATER]
    ) {
      boardData.maxComboCount += 3
      if (firstTime) {
        boardData.maxComboCount -= 2
      }
    }
    break
  case VEROAH:
    if (9 <= boardData['dropCountArray'][WATER]) {
      boardData.maxComboCount += 3
      if (firstTime) {
        boardData.maxComboCount -= 2
      }
    }
    break
  case AMAKOZUMI:
    if (
      3 <= boardData['dropCountArray'][LIGHT] &&
      3 <= boardData['dropCountArray'][DARK]
    ) {
      boardData.maxComboCount += 3
    }
    break
  case SUKUNA:
    if (
      3 <= boardData['dropCountArray'][FIRE]
    ) {
      boardData.maxComboCount += 4
    }
    break
  }
}

// 盤面で可能なリーダースキルの最大倍率を適用する関数
function multiplyMagnification(boardData, leader) {
  const unclearableDrops = boardData['dropCountArray'].filter(num => num < 3)
  const unclearableDropSum = unclearableDrops.length ?
                             unclearableDrops.reduce((a,b) => a+b) : 0
  switch (leader) {
  // 転生アヌビス：
  // 8コンボ以上で攻撃力が上昇、最大10倍。
  // スキル使用時、攻撃力が3倍、回復力は1.5倍。
  case ANUBIS:
    if (7 >= boardData['maxComboCount'])       boardData['maxMagnification'] *= 3
    else if (8 === boardData['maxComboCount']) boardData['maxMagnification'] *= 15
    else if (9 === boardData['maxComboCount']) boardData['maxMagnification'] *= 22.5
    else if (10 <= boardData['maxComboCount']) boardData['maxMagnification'] *= 30
    break
  // 極醒メタトロン：
  // HP80%以下で攻撃力が3倍。神タイプのHPと攻撃力が2倍。
  // 7コンボ以上でダメージを軽減、攻撃力が2.5倍。
  case METATRON:
    boardData['maxMagnification'] *= 2
    if (boardData['maxComboCount'] >= 7) {
      boardData['maxMagnification'] *= 2.5
    }
    break
  // コマさんS：
  // 火と木属性のHPが1.5倍。6コンボ以上で攻撃力が5倍。
  // ドロップを5個以上つなげて消すとダメージを軽減、攻撃力が3倍。
  case KOMASAN:
    if (boardData['maxComboCount'] >= 6) {
      boardData['maxMagnification'] *= 5
    }
    if (boardData['mostDropCount'] >= 5) {
      boardData['maxMagnification'] *= 3
    }
    break
  // アメン：
  // 【落ちコンなし】 7コンボちょうどで攻撃力が10倍。
  // パズル後の残りドロップ数が3個以下で攻撃力が10倍。
  case AMEN:
    if (boardData['maxComboCount'] >= 7) {
      boardData['maxMagnification'] *= 10
    }
    if (unclearableDropSum <= 3) {
      boardData['maxMagnification'] *= 10
    }
    break
  // ハイレン：
  // 水属性のHPが2倍。10コンボ以上で攻撃力が30倍。
  // 火と水を同時に7個以上つなげて消すと、3コンボ加算。
  case HYLEN:
    if (boardData['maxComboCount'] >= 10) {
      boardData['maxMagnification'] *= 30
    }
    break
  // ココ・フェルケナ：
  // 水属性の攻撃力が4倍。6コンボ以上でダメージを半減。
  // 水を6個以上つなげて消すと攻撃力が4倍、固定5万ダメージ。
  case COCO:
    boardData['maxMagnification'] *= 4
    if (boardData['dropCountArray'][2] >= 6) {
      boardData['maxMagnification'] *= 4
    }
    break
  // 水着ヴェロア：
  // 水属性のHPが2倍、攻撃力は6倍。ドロップ操作を2秒延長。
  // 水を9個以上つなげて消すと攻撃力が3倍、3コンボ加算。
  case VEROAH:
    boardData['maxMagnification'] *= 6
    if (boardData['dropCountArray'][2] >= 9) {
      boardData['maxMagnification'] *= 3
    }
    break
  // 鞍馬夜叉丸：
  // 【落ちコンなし】HPと回復力が2倍。6コンボ以上で攻撃力が3倍。
  // パズル後の残りドロップ数が15個以下で攻撃力が上昇、最大9倍。
  case YASHAMARU:
    if (boardData['maxComboCount'] >= 6) {
      boardData['maxMagnification'] *= 3
    }
    boardData['maxMagnification'] *= Math.max(9 - 0.5 * unclearableDropSum, 1)
    break
  // 色：
  // 闇属性のHPが2倍。ドロップの5個L字消しで攻撃力が5倍。
  // 4色以上同時攻撃で攻撃力が3倍、固定5万ダメージ。
  case SHIKI:
    let clearableLFlag = false
    let clearableColorsCount = 0
    for (let color = FIRE; color <= DROP_TYPE_MAX; color++) {
      const dropCount = boardData['dropCountArray'][color]
      if (dropCount >= 5) {
        clearableLFlag = true
      }
      if (color <= 5 && dropCount >= 3) {
        clearableColorsCount++
      }
    }
    if (clearableLFlag) {
      boardData['maxMagnification'] *= 5
    }
    if (clearableColorsCount >= 4) {
      boardData['maxMagnification'] *= 3
    }
    break
  case AMAKOZUMI:
    let crossCount = 0
    if (
      3 <= boardData['dropCountArray'][LIGHT] &&
      3 <= boardData['dropCountArray'][DARK]
    ) {
      boardData['maxMagnification'] *= 3
    }
    for (let color = FIRE; color <= DROP_TYPE_MAX; color++) {
      const dropCount = boardData['dropCountArray'][color]
      crossCount += Math.floor(dropCount / 5)
    }
    if (boardData.boardSize === '5x6') {
      crossCount = Math.min(3, crossCount)
    } else if (boardData.boardSize === '6x7') {
      crossCount = Math.min(3, crossCount)
    }
    boardData['maxMagnification'] *= Math.pow(3, crossCount)
    break
  case WRATH:
    boardData['maxMagnification'] *= 16
    break
  // カイロ・レン
  // 火闇の同時攻撃でダメージを65％軽減、攻撃力が5倍、
  // 固定1ダメージ。火か闇の5個十字消し1個につき攻撃力が3倍。
  case KYLO_REN:
    {
      const fireDropCount = boardData['dropCountArray'][FIRE]
      const darkDropCount = boardData['dropCountArray'][DARK]
      const crossLimit = boardData['boardSize'] === '5x6' ? 3 : 6
      const fireCrossCount = Math.floor(fireDropCount / 5)
      const darkCrossCount = Math.floor(darkDropCount / 5)
      const crossCountAmount = Math.min(crossLimit, fireCrossCount + darkCrossCount)
  
      if (fireDropCount >= 3 && darkDropCount >= 3) {
        boardData['maxMagnification'] *= 5
      }
  
      boardData['maxMagnification'] *= Math.pow(3, crossCountAmount)
    }
    break
  // アルラトゥ
  // 【7x6マス】闇属性の回復力が3倍。操作時間5秒延長。
  // 火か水か闇の5個十字消し1個につき攻撃力が3倍。
  case ALLATU:
    {
      const fireDropCount = boardData['dropCountArray'][FIRE]
      const waterDropCount = boardData['dropCountArray'][WATER]
      const darkDropCount = boardData['dropCountArray'][DARK]
      const crossLimit = boardData['boardSize'] === '5x6' ? 3 : 6
      const fireCrossCount = Math.floor(fireDropCount / 5)
      const waterCrossCount = Math.floor(waterDropCount / 5)
      const darkCrossCount = Math.floor(darkDropCount / 5)
      const crossCountAmount = Math.min(crossLimit, fireCrossCount + waterCrossCount + darkCrossCount)

      boardData['maxMagnification'] *= Math.pow(3, crossCountAmount)
    }
    break
  // 両面宿儺：
  // 闇を5個以上つなげて消すとダメージを81%軽減、攻撃力が50倍、
  // 固定1000万ダメージ。火ドロップを消すと、4コンボ加算。
  case SUKUNA:
    const darkDropCount = boardData['dropCountArray'][DARK]
    if (darkDropCount >= 5) {
      boardData['maxMagnification'] *= 50
    }
    break
  }
}


// 盤面を分析してオブジェクトにする関数
function getBoardData(board) {

  // 盤面のサイズ
  const boardSize = (board.length === 30) ? '5x6' : '6x7'

  // 盤面に存在するドロップの数(種類別)
  const dropCountArray = (new Array(DROP_TYPE_MAX + 1)).fill(0)

  // 盤面を見て、その色に対応するインデックスの値をインクリメント
  board.forEach(color => dropCountArray[color]++)

  // 存在するドロップだけ残した配列
  const existDropCountArray = dropCountArray.filter(num => num)

  // 盤面に存在するドロップの種類数
  const dropTypeCount = existDropCountArray.length

  // 最も多いドロップの数
  const mostDropCount = Math.max.apply(null, existDropCountArray)

  // 最も少ないドロップの数
  const fewestDropCount = Math.min.apply(null, existDropCountArray)

  // ドロップの種類別の、可能な最大コンボ数（3で割るだけ）
  const maxComboCountArray = dropCountArray.map(num => Math.floor(num / 3))

  // ドロップの種類別の、可能な最大コンボ数（5で割るだけ）
  const maxCrossCountArray = dropCountArray.map(num => Math.floor(num / 5))

  // ドロップの種類数に応じて可能な最大コンボ数・十字消し数のそれぞれの和を求める
  let maxComboCount = null
  let maxCrossCount = null
  switch (maxComboCount) {
  case 1:
    maxComboCount = 1
    maxCrossCount = 0
    break
  case 2:
    maxComboCount = getMaxComboOfTwoColorBoard(board.length, fewestDropCount)
    maxCrossCount = getMaxCrossOfTwoColorBoard(board.length, fewestDropCount)
    break
  default:
    maxComboCount = maxComboCountArray.reduce((a, b) => a + b)
    maxCrossCount = maxCrossCountArray.reduce((a, b) => a + b)
  }
  const maxMagnification = 1

  return {
    boardSize,
    dropCountArray,
    existDropCountArray,
    dropTypeCount,
    mostDropCount,
    fewestDropCount,
    maxComboCountArray,
    maxComboCount,
    maxCrossCount,
    maxMagnification,
  }
}


// ２色盤面での最大コンボ数を返す関数
function getMaxComboOfTwoColorBoard(boardLength, fewDropCount) {
  if (boardLength === (5 * 6)) {
    switch (fewDropCount) {
      case 1  :
      case 2  : return 1
      case 3  : return 2
      case 4  : return 3
      case 5  : return 4
      case 6  : return 5
      case 7  : return 6
      case 8  : return 7
      case 9  :
      case 10 : return 8
      case 11 :
      case 12 :
      case 13 :
      case 14 : return 9
      case 15 : return 10
      default : return 0
    }
  } else if (boardLength === (6 * 7)) {
    switch (fewDropCount) {
      case 1  :
      case 2  : return 1
      case 3  : return 2
      case 4  :
      case 5  : return 3
      case 6  : return 5
      case 14 :
      case 15 :
      case 16 :
      case 17 : return 13
      case 18 : return 14
      case 19 :
      case 20 : return 13
      case 21 : return 14
      default : return 12 // 7~13はデータがないからとりあえず12
    }
  } else {
    return 0
  }
}

// 盤面で可能な十字消しの個数を求める関数(2色陣)
function getMaxCrossOfTwoColorBoard(boardLength, fewDropCount) {
  switch (boardLength) {
  case 30:
    if (fewDropCount <= 4)       return 0
    else if (fewDropCount <= 6)  return 1
    else if (fewDropCount <= 11) return 2
    else                         return 3
    break
  case 42:
    if (fewDropCount <= 4)       return 0
    else if (fewDropCount <= 6)  return 1
    else if (fewDropCount <= 11) return 2
    else if (fewDropCount <= 13) return 3
    else if (fewDropCount <= 15) return 4
    else if (fewDropCount <= 17) return 5
    else                         return 6
    break
  default:
    return 0
  }
}

// 盤面で可能な十字消しの個数を求める関数(3色陣)
// function getMaxCrossOfThreeColorBoard(boardLength, fewDropCount) {
//   let maxCrossCount = null

//   maxCrossCount = Math.floor(color1Count / 5)
//                 + Math.floor(color2Count / 5)
//   switch (boardLength) {
//     case "5x6": maxCrossCount = Math.min(3, maxCrossCount); break
//     case "6x7": maxCrossCount = Math.min(6, maxCrossCount); break
//   }

//   return maxCrossCount
// }
