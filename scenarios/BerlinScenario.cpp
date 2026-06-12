#include "BerlinScenario.h"
#include "../core/StoryNode.h"

QString BerlinScenario::startNodeId(PlayerClass) const {
    return QStringLiteral("be_intro");
}

void BerlinScenario::initialize() {
    // ========================================================
    // 节点 1: 柏林介绍 (叙事)
    // ========================================================
    {
        StoryNode n;
        n.id            = QStringLiteral("be_intro");
        n.locationTitle = QStringLiteral("1945年4月16日 · 柏林，帝国的黄昏");
        n.musicKey      = QStringLiteral("berlin");
        n.type          = NodeType::Narrative;
        n.text =
            QStringLiteral("1945年4月。柏林已是一片焦土。\n\n"
            "曾经自诩能统治千年的第三帝国，如今只剩下一片残破的骨架。\n"
            "盟军的大举轰炸已将整座城市夷为平地，而在东线，白俄罗斯第一方面军和乌克兰第一方面军的近两百万红军士兵、万余门火炮、数千辆坦克正在奥德河畔集结，准备发起对柏林的最后一击。\n\n"
            "防空洞里的市民面如死灰，废墟中的瓦砾在持续不断的远程炮击下微微颤抖。\n\n"
            "作为一名在经历了无数次战斗后侥幸存活的德军士兵，你站在摇摇欲坠的街角，看着满天的烟尘，知道最后的结局已经不可逆转。");
        n.nextNodeId = QStringLiteral("be_assignment");
        addNode(n);
    }

    // ========================================================
    // 节点 2: 分配防区 (选择)
    // ========================================================
    {
        StoryNode n;
        n.id            = QStringLiteral("be_assignment");
        n.locationTitle = QStringLiteral("1945年4月16日 · 柏林，命令分派");
        n.musicKey      = QStringLiteral("berlin");
        n.type          = NodeType::Choice;
        n.text =
            QStringLiteral("最后的集结号吹响了。防区军官正在沙盘前分配任务。\n\n"
            "对于不同的兵种，最后的任务也各不相同。你必须服从最后的宿命，或者选择在这场疯狂中尽力保全自己。");

        Choice c1;
        c1.text             = QStringLiteral("【东部防线】前往奥德河防线抵抗红军");
        c1.classRestricted  = true;
        c1.allowedClasses   = QList<PlayerClass>{PlayerClass::Infantry, PlayerClass::TankCrew};
        c1.nextNodeId       = QStringLiteral("be_oder_front");

        Choice c2;
        c2.text             = QStringLiteral("【防空塔】前往制空阵地防空塔驻防");
        c2.classRestricted  = true;
        c2.allowedClasses   = QList<PlayerClass>{PlayerClass::FighterPilot, PlayerClass::BomberPilot};
        c2.nextNodeId       = QStringLiteral("be_air_defense");

        Choice c3;
        c3.text             = QStringLiteral("【施普雷河】封锁施普雷河防御阵线");
        c3.classRestricted  = true;
        c3.allowedClasses   = QList<PlayerClass>{PlayerClass::SubmarineCrew, PlayerClass::BattleshipCrew};
        c3.nextNodeId       = QStringLiteral("be_river_defense");

        Choice c4;
        c4.text             = QStringLiteral("【服从分配】服从命令分配，驻守城区废墟");
        c4.nextNodeId       = QStringLiteral("be_city_center");

        n.choices = QList<Choice>{c1, c2, c3, c4};
        addNode(n);
    }

    // ========================================================
    // 节点 3: 奥德河防线 (选择 - 步兵/坦克)
    // ========================================================
    {
        StoryNode n;
        n.id            = QStringLiteral("be_oder_front");
        n.locationTitle = QStringLiteral("1945年4月18日 · 奥德河防线");
        n.musicKey      = QStringLiteral("berlin");
        n.type          = NodeType::Choice;
        n.text =
            QStringLiteral("奥德河畔，苏联红军的喀秋莎火箭炮铺天盖地。\n"
            "阵地上的泥土被炸得翻了一遍又一遍，夹杂着焦黑的碎肉。\n\n"
            "你所在的防线正在崩溃，苏联坦克像钢铁巨兽般隆隆驶来，碾碎了一道又一道堑壕。\n"
            "你手里的武器只有一支Kar98k步枪，或者一具铁拳反坦克火箭筒。这是毫无胜算的防守。");

        Choice c1;
        c1.text             = QStringLiteral("【伏击坦克】使用铁拳火箭筒近距离伏击苏军坦克");
        c1.isCombat         = true;
        c1.combatThreshold  = 55;
        c1.bonusClasses     = QList<PlayerClass>{PlayerClass::Infantry, PlayerClass::TankCrew};
        c1.successNodeId    = QStringLiteral("be_tank_kill");
        c1.failureNodeId    = QStringLiteral("be_wounded");
        c1.failHpDelta      = -35;
        c1.failMoraleDelta  = -15;

        Choice c2;
        c2.text             = QStringLiteral("【撤退】放弃阵地，跟随溃兵撤向市区");
        c2.hpDelta          = -10;
        c2.moraleDelta      = -15;
        c2.nextNodeId       = QStringLiteral("be_oder_retreat");

        n.choices = QList<Choice>{c1, c2};
        addNode(n);
    }

    // ========================================================
    // 节点 4: 坦克摧毁 (叙事)
    // ========================================================
    {
        StoryNode n;
        n.id            = QStringLiteral("be_tank_kill");
        n.locationTitle = QStringLiteral("1945年4月18日 · 奥德河防线");
        n.musicKey      = QStringLiteral("berlin");
        n.type          = NodeType::Narrative;
        n.text =
            QStringLiteral("你深吸一口气，顶着漫天弹雨探出战壕。铁拳反坦克火箭筒在肩头喷射出尾焰，一枚高爆弹头狠狠砸在了苏军T-34坦克的侧甲上，坦克瞬间化为一团烈火。\n\n"
            "但还没等你们喘口气，更多的坦克轮廓从硝烟中显现出来。你身边的同伴被坦克车载机枪子弹拦腰扫断。\n\n"
            "击毁一辆坦克是毫无意义的——他们有成千上万辆，而你只有这一具铁拳。\n"
            "眼看防线彻底失守，你只能在战友的残骸中寻找掩护，撤向柏林市区。");
        n.nextNodeId = QStringLiteral("be_city_center");
        addNode(n);
    }

    // ========================================================
    // 节点 5: 受伤 (叙事)
    // ========================================================
    {
        StoryNode n;
        n.id            = QStringLiteral("be_wounded");
        n.locationTitle = QStringLiteral("1945年4月18日 · 奥德河防线");
        n.musicKey      = QStringLiteral("berlin");
        n.type          = NodeType::Narrative;
        n.text =
            QStringLiteral("你刚想站起来，一颗重炮炮弹在不远处炸开。巨大的气浪和漫天的泥土瞬间将你掩埋。\n\n"
            "你只觉得左臂一凉，随后是钻心的剧痛。弹片割开了你的军装，鲜血汩汩流出。\n\n"
            "当你挣扎着从泥土里爬出来时，排长和机枪手都已没有了呼吸，空气里充满了焦糊的气味。\n"
            "你用绷带简单包扎了伤口，拖着伤痕累累的身体，在红军火力的追击下艰难撤向市区。");
        n.nextNodeId = QStringLiteral("be_city_center");
        addNode(n);
    }

    // ========================================================
    // 节点 6: 撤退 (叙事)
    // ========================================================
    {
        StoryNode n;
        n.id            = QStringLiteral("be_oder_retreat");
        n.locationTitle = QStringLiteral("1945年4月18日 · 奥德河防线外围");
        n.musicKey      = QStringLiteral("berlin");
        n.type          = NodeType::Narrative;
        n.text =
            QStringLiteral("你选择了后退。军官的皮鞭和手枪已经无法阻挡士兵们的溃逃。\n\n"
            "在你们身后，红军的喀秋莎齐射将整片平原化为火海，成百上千来不及撤退的战友在火光中化为灰烬。\n\n"
            "你一路狂奔，甚至扔掉了水壶和防毒面具，直到在柏林郊区残存的街道里气喘吁吁地停下。\n"
            "看着一路上丢弃的装备和神色呆滞的溃兵，你感到了无言的绝望。这已经是最后的防线了。");
        n.nextNodeId = QStringLiteral("be_city_center");
        addNode(n);
    }

    // ========================================================
    // 节点 7: 防空塔 (选择 - 飞行员)
    // ========================================================
    {
        StoryNode n;
        n.id            = QStringLiteral("be_air_defense");
        n.locationTitle = QStringLiteral("1945年4月20日 · 柏林防空塔");
        n.musicKey      = QStringLiteral("berlin");
        n.type          = NodeType::Choice;
        n.text =
            QStringLiteral("柏林防空塔是城市里最坚固的混凝土堡垒，拥有双联装128毫米高射炮和厚达数米的墙体。\n"
            "对于飞行员来说，没有了飞机和油料，这里就是你们最后的阵地。\n\n"
            "天空已经被红军的雅克战机和苏美轰炸机完全主宰。高射炮的怒吼不过是垂死挣扎。\n"
            "红军步兵已经开始逼近防空塔，战局正在从防空转入残酷的地面堡垒守备。");

        Choice c1;
        c1.text             = QStringLiteral("【高炮平射】将高射炮指向地面，压制苏军地面步兵");
        c1.isCombat         = true;
        c1.combatThreshold  = 50;
        c1.bonusClasses     = QList<PlayerClass>{PlayerClass::FighterPilot, PlayerClass::BomberPilot};
        c1.successNodeId    = QStringLiteral("be_flak_hold");
        c1.failureNodeId    = QStringLiteral("be_flak_destroyed");
        c1.failHpDelta      = -30;
        c1.failMoraleDelta  = -20;

        Choice c2;
        c2.text             = QStringLiteral("【撤入通道】放弃重装备，撤入防空塔地底的避难坑道");
        c2.nextNodeId       = QStringLiteral("be_city_center");

        n.choices = QList<Choice>{c1, c2};
        addNode(n);
    }

    // ========================================================
    // 节点 8: 高炮平射成功 (叙事)
    // ========================================================
    {
        StoryNode n;
        n.id            = QStringLiteral("be_flak_hold");
        n.locationTitle = QStringLiteral("1945年4月20日 · 柏林防空塔顶");
        n.musicKey      = QStringLiteral("berlin");
        n.type          = NodeType::Narrative;
        n.text =
            QStringLiteral("你们将重型高射炮的炮口降下，指向地面的街道。128毫米双联装高射炮弹呼啸着在街区废墟中炸开，将正在推进的红军士兵和装甲车撕成碎片。\n\n"
            "这威力极其恐怖，但你没有一丝快感。\n"
            "你看到那些在血雾中倒下的红军士兵，他们也是某些人的儿子、丈夫和父亲。他们历经千辛万苦打到这里，却在战争结束前夕死于水泥怪物的平射下。\n\n"
            "最终，苏军使用大口径重炮对防空塔实施了报复性轰击。防空塔被浓烟和烈火笼罩，你不得不撤入地下。");
        n.nextNodeId = QStringLiteral("be_city_center");
        addNode(n);
    }

    // ========================================================
    // 节点 9: 高炮阵地被毁 (叙事)
    // ========================================================
    {
        StoryNode n;
        n.id            = QStringLiteral("be_flak_destroyed");
        n.locationTitle = QStringLiteral("1945年4月20日 · 柏林防空塔顶");
        n.musicKey      = QStringLiteral("berlin");
        n.type          = NodeType::Narrative;
        n.text =
            QStringLiteral("一颗苏联的203毫米重榴弹炮弹直接命中了高炮阵地。\n\n"
            "伴随着震耳欲聋的爆炸，重达数吨的高射炮管被生生折断，大块的水泥碎屑如冰雹般砸下，将几名炮手当场砸死在血泊中。\n"
            "你的头部撞在了钢铁栏杆上，鲜血模糊了眼睛，耳边是一片尖锐的鸣叫声。\n\n"
            "在防空塔被彻底孤立并沦为瓦砾堆之前，残存的士兵扶着你顺着阴暗的楼梯逃往地下掩体。");
        n.nextNodeId = QStringLiteral("be_city_center");
        addNode(n);
    }

    // ========================================================
    // 节点 10: 施普雷河防御线 (选择 - 海军)
    // ========================================================
    {
        StoryNode n;
        n.id            = QStringLiteral("be_river_defense");
        n.locationTitle = QStringLiteral("1945年4月22日 · 施普雷河防御线");
        n.musicKey      = QStringLiteral("berlin");
        n.type          = NodeType::Choice;
        n.text =
            QStringLiteral("施普雷河是柏林市中心前的最后一道屏障。\n"
            "对于海军人员来说，战列舰已经沉没，潜艇早已在海战中折戟，现在你们只能在河岸的断桥残壁旁扮演步兵的角色。\n\n"
            "河水早已被弹药和废弃物污染，散发着恶臭。\n"
            "苏军工兵正顶着弹雨在对岸架设浮桥，红军的突击艇正在强行渡河。防线危在旦夕。");

        Choice c1;
        c1.text             = QStringLiteral("【射击渡船】用残存的机枪火力扫射渡河的苏军突击艇");
        c1.isCombat         = true;
        c1.combatThreshold  = 50;
        c1.bonusClasses     = QList<PlayerClass>{PlayerClass::SubmarineCrew, PlayerClass::BattleshipCrew};
        c1.successNodeId    = QStringLiteral("be_spree_repelled");
        c1.failureNodeId    = QStringLiteral("be_spree_broken");
        c1.failHpDelta      = -25;
        c1.failMoraleDelta  = -15;

        Choice c2;
        c2.text             = QStringLiteral("【炸毁桥梁】提前炸毁施普雷河上的桥梁以阻滞苏军");
        c2.nextNodeId       = QStringLiteral("be_spree_destroyed");

        n.choices = QList<Choice>{c1, c2};
        addNode(n);
    }

    // ========================================================
    // 节点 11: 扫射成功 (叙事)
    // ========================================================
    {
        StoryNode n;
        n.id            = QStringLiteral("be_spree_repelled");
        n.locationTitle = QStringLiteral("1945年4月22日 · 施普雷河西岸");
        n.musicKey      = QStringLiteral("berlin");
        n.type          = NodeType::Narrative;
        n.text =
            QStringLiteral("机枪的狂吼在河谷间回荡。你死死压着枪身，子弹扫过河面，打穿了皮划艇，将艇上的红军士兵成排扫落到冰冷的河水中。\n\n"
            "河面上升起了一团团血红色的水花。那些士兵在水中挣扎，沉没，最终被肮脏的河水卷走。\n\n"
            "你们勉强击退了这一波进攻，但弹药已经见底。\n"
            "苏军火炮随即开始了犁地般的轰击，河岸阵地在炮火中荡然无存。你只能撤入城区废墟。");
        n.nextNodeId = QStringLiteral("be_city_center");
        addNode(n);
    }

    // ========================================================
    // 节点 12: 防线失守 (叙事)
    // ========================================================
    {
        StoryNode n;
        n.id            = QStringLiteral("be_spree_broken");
        n.locationTitle = QStringLiteral("1945年4月22日 · 施普雷河西岸");
        n.musicKey      = QStringLiteral("berlin");
        n.type          = NodeType::Narrative;
        n.text =
            QStringLiteral("防线被彻底撕碎了。苏联红军的装甲车辆顶着火力冲上了西岸，冲锋枪子弹将你们的掩体打得千疮百孔。\n\n"
            "一名红军士兵端着波波沙冲锋枪跳进你的沙袋掩体，你被迫用工兵铲和他进行肉搏，在血水中拼命撕扯。\n\n"
            "虽然你侥幸逃脱，但腹部和腿部多处负伤。施普雷河防线宣告失守，你被溃败的人流裹挟着退向市中心。");
        n.nextNodeId = QStringLiteral("be_city_center");
        addNode(n);
    }

    // ========================================================
    // 节点 13: 炸毁桥梁 (叙事)
    // ========================================================
    {
        StoryNode n;
        n.id            = QStringLiteral("be_spree_destroyed");
        n.locationTitle = QStringLiteral("1945年4月22日 · 施普雷河桥头");
        n.musicKey      = QStringLiteral("berlin");
        n.type          = NodeType::Narrative;
        n.text =
            QStringLiteral("你按下了起爆器。伴随着剧烈的爆炸，残存的石桥在一片烟尘中坍塌进施普雷河，激起巨大的水流。\n\n"
            "然而，在河对岸，还有来不及撤回的数十名德军士兵和数百名柏林平民。\n"
            "他们惊恐地看着断桥，在他们身后，是红军的装甲洪流。哭喊声、求救声隔着河水传来，接着是密集的枪声。\n\n"
            "你不敢回头去看河对岸发生的惨剧。这毫无疑问是用战友和平民的生命在拖延死亡。你的士气降到了谷底。");
        n.nextNodeId = QStringLiteral("be_city_center");
        addNode(n);
    }

    // ========================================================
    // 节点 14: 市中心废墟 (选择 - 孩子)
    // ========================================================
    {
        StoryNode n;
        n.id            = QStringLiteral("be_city_center");
        n.locationTitle = QStringLiteral("1945年4月25日 · 柏林，市中心废墟");
        n.musicKey      = QStringLiteral("berlin");
        n.type          = NodeType::Choice;
        n.text =
            QStringLiteral("柏林市区已经沦为绞肉机。无论你之前来自什么防区，现在都被混编在一起，进行残酷的街垒战。\n"
            "街道两侧挂满了被宪兵处决的「逃兵」的尸体，胸前写着「我是个懦夫」的牌子在风中摇晃。\n\n"
            "在一处断壁残垣后，你遇到了你的「新战友」——\n"
            "他穿着松垮的希特勒青年团制服，钢盔大得遮住了他的眼睛，身体在剧烈地颤抖。他手里抱着一具铁拳，指关节因为用力过度而发白。\n"
            "他看起来最多只有十三四岁，还是个孩子。\n\n"
            "「长官，」他的声音带着哭腔，「苏联坦克要过来了，我该怎么办？」");

        Choice c1;
        c1.text             = QStringLiteral("【劝其回家】把铁拳拿过来，劝他脱掉制服回家，替他防守这里");
        c1.moraleDelta      = 15;
        c1.hpDelta          = -10;
        c1.grantedFlags     = QSet<QString>{QStringLiteral("saved_boy")};
        c1.nextNodeId       = QStringLiteral("be_reichstag_saved");

        Choice c2;
        c2.text             = QStringLiteral("【命令防守】严厉命令他坚守岗位，履行军人职责");
        c2.moraleDelta      = -25;
        c2.nextNodeId       = QStringLiteral("be_reichstag_not_saved");

        Choice c3;
        c3.text             = QStringLiteral("【视而不见】默默从他身边走过，不去管这个孩子的命运");
        c3.moraleDelta      = -10;
        c3.nextNodeId       = QStringLiteral("be_reichstag_not_saved");

        n.choices = QList<Choice>{c1, c2, c3};
        addNode(n);
    }

    // ========================================================
    // 节点 15: 孩子回家 (叙事)
    // ========================================================
    {
        StoryNode n;
        n.id            = QStringLiteral("be_reichstag_saved");
        n.locationTitle = QStringLiteral("1945年4月28日 · 国会大厦周边");
        n.musicKey      = QStringLiteral("berlin");
        n.type          = NodeType::Narrative;
        n.text =
            QStringLiteral("战事已经蔓延到了国会大厦外围的国王广场。\n"
            "苏军的重炮日夜不停地轰击着这片焦土，空气中充斥着炸药、腐肉和焦土的混合气味。你躲在一个弹坑里，看着残缺不全的勃兰登堡门。\n\n"
            "你看到废墟中没有那个男孩的身影，心中暗自祈祷他已经脱下制服，躲回了母亲的地下室里，能够在即将到来的停火中活下来。\n"
            "但在你周围，依然有成千上万像他一样的孩子，正被当作炮灰填进战争的最后一炉火中。\n\n"
            "你握紧步枪，明白这场毫无意义的战争正在吞噬民族最后的未来。");
        n.nextNodeId = QStringLiteral("be_fuhrer_death");
        addNode(n);
    }

    // ========================================================
    // 节点 16: 孩子战死 (叙事)
    // ========================================================
    {
        StoryNode n;
        n.id            = QStringLiteral("be_reichstag_not_saved");
        n.locationTitle = QStringLiteral("1945年4月28日 · 国会大厦周边");
        n.musicKey      = QStringLiteral("berlin");
        n.type          = NodeType::Narrative;
        n.text =
            QStringLiteral("国王广场已是人间地狱，到处都是在炮击下飞舞的水泥碎屑和人体残骸。\n"
            "你在撤退到一个新的弹坑时，几乎被地上的障碍物绊倒。\n\n"
            "你低下头，看到了那具小小的尸体。\n"
            "正是那个小男孩。他大大的钢盔掉在一旁，金色的头发被血水粘在额头上。他的腹部被子弹撕开，手里依然死死抓着那具没有发射出去的铁拳，眼睛空洞地望着灰蒙蒙的天空。\n\n"
            "这具弱小的躯体，成为了这栋千疮百孔的帝国大厦前无数无名墓碑中的一个。\n"
            "你感到了深深的负罪感和荒谬。这绝不是什么荣耀，这是对生命的屠杀。");
        n.nextNodeId = QStringLiteral("be_fuhrer_death");
        addNode(n);
    }

    // ========================================================
    // 节点 17: 元首自杀 (叙事)
    // ========================================================
    {
        StoryNode n;
        n.id            = QStringLiteral("be_fuhrer_death");
        n.locationTitle = QStringLiteral("1945年4月30日 · 元首地堡外围");
        n.musicKey      = QStringLiteral("berlin_elegy");
        n.type          = NodeType::Narrative;
        n.text =
            QStringLiteral("1945年4月30日下午。\n"
            "一个震惊的消息在守军废墟间以耳语的形式传播：元首在总理府地堡内自杀了。\n"
            "那个曾经用高亢演讲将整代德国人推向战场的人，那个声称要战至最后一人的人，用一颗子弹在地下深处结束了他自己的生命。\n\n"
            "废墟陷入了诡异的死寂，只有苏联大炮的轰鸣依然在有节奏地回响。\n"
            "周围的士兵有的跪在地上痛哭，有的愤怒地撕下领章，有的则像行尸走肉般盯着空洞的火光。\n\n"
            "一切都结束了。所有的口号、所有的宏图，最终只剩下了这片燃烧的废墟，和死无葬身之地的千百万亡魂。\n"
            "接下来，是你们自己做决定的时候了。");
        n.nextNodeId = QStringLiteral("be_final_choice");
        addNode(n);
    }

    // ========================================================
    // 节点 18: 最后的抉择 (选择)
    // ========================================================
    {
        StoryNode n;
        n.id            = QStringLiteral("be_final_choice");
        n.locationTitle = QStringLiteral("1945年5月1日 · 柏林，最后的抉择");
        n.musicKey      = QStringLiteral("berlin_elegy");
        n.type          = NodeType::Choice;
        n.text =
            QStringLiteral("红军的红旗已经插上了国会大厦的穹顶。柏林防区司令魏德林正在准备投降命令。\n"
            "废墟的各个角落里，残存的德军士兵正在面临最后的命运。\n\n"
            "命令已经不复存在，誓言已化为泡影。在这个充斥着硝烟的清晨，你必须做出你人生中最后一个决定。");

        Choice c1;
        c1.text             = QStringLiteral("【向东投降】走出废墟，向苏联红军举手投降");
        c1.nextNodeId       = QStringLiteral("be_surrender_soviet");

        Choice c2;
        c2.text             = QStringLiteral("【向西突围】尝试穿越市区的包围圈，向西逃亡以投降美英盟军");
        c2.isCombat         = true;
        c2.combatThreshold  = 50;
        c2.successNodeId    = QStringLiteral("be_west_escape");
        c2.failureNodeId    = QStringLiteral("be_captured");
        c2.failHpDelta      = -25;
        c2.failMoraleDelta  = -20;

        Choice c3;
        c3.text             = QStringLiteral("【隐蔽等待】扔掉武器，躲避在平民地窖或废墟地道里等待终战");
        c3.nextNodeId       = QStringLiteral("be_wait_end");

        n.choices = QList<Choice>{c1, c2, c3};
        addNode(n);
    }

    // ========================================================
    // 节点 19: 结局 — 向东投降 (Ending)
    // ========================================================
    {
        StoryNode n;
        n.id            = QStringLiteral("be_surrender_soviet");
        n.locationTitle = QStringLiteral("柏林战役 — 结束（向东投降）");
        n.musicKey      = QStringLiteral("berlin_end");
        n.type          = NodeType::Ending;
        n.isVictory     = true;
        n.text =
            QStringLiteral("【战役结束 — 存活】\n\n"
            "你扔掉了武器，走出摇摇欲坠的地下室，将双手高高举起。\n"
            "烟雾中，几名端着波波沙冲锋枪的苏联红军士兵向你走来。他们粗暴地搜了你的身，拿走了你的手表和勋章，但没有开枪。\n\n"
            "你被编入一眼望不到头的战俘队伍，在废墟瓦砾间缓缓向前移动。路边，市民们正在抢夺死马的肉，柏林的街道上横七竖八地躺着年轻守备队员和苏联士兵的尸体。\n"
            "你活了下来，虽然你即将面对的是漫长而残酷的西伯利亚战俘营岁月——在那里，很多人将无法活过第一个冬天。但至少，你不需要再为那个疯狂的战争机器流血了。\n\n"
            "────────────────\n\n"
            "第二次世界大战欧洲战场以纳粹德国的无条件投降而告终。\n"
            "这场战争造成了人类历史上空前的浩劫，仅欧洲战场的死亡人数就高达四千万至五千万人，全球总伤亡人数估计达七千万至八千五百万人。\n"
            "战争并无真正的荣耀，唯有无尽的废墟，和生者漫长的救赎与反思。\n\n"
            "感谢通关本游戏。愿世界永无战争。");
        addNode(n);
    }

    // ========================================================
    // 节点 20: 结局 — 向西突围 (Ending)
    // ========================================================
    {
        StoryNode n;
        n.id            = QStringLiteral("be_west_escape");
        n.locationTitle = QStringLiteral("柏林战役 — 结束（向西突围）");
        n.musicKey      = QStringLiteral("berlin_end");
        n.type          = NodeType::Ending;
        n.isVictory     = true;
        n.text =
            QStringLiteral("【战役结束 — 存活】\n\n"
            "你在黑夜的掩护下，穿过了燃烧的街道，越过施普雷河上的桥梁残骸，向西狂奔。\n"
            "子弹在你耳边嗖嗖作响，不断有同伴在身边惨叫着倒下，但你没有回头。\n\n"
            "经历了整整一天的逃亡和躲避，你终于跨过了易北河的缓冲带，看到了前方穿着土黄色军装的美军哨兵。\n"
            "你丢下刺刀和皮带，举起双手走向他们。一名年轻的美军士兵搜查了你，递给你一支香烟。你坐在吉普车旁，看着东方升起的朝阳，眼泪止不住地流下。\n"
            "你活下来了。战争结束了。你的家乡或许已经不复存在，但你还有机会去重新开始人生。\n\n"
            "────────────────\n\n"
            "第二次世界大战欧洲战场以纳粹德国的无条件投降而告终。\n"
            "这场战争造成了人类历史上空前的浩劫，仅欧洲战场的死亡人数就高达四千万至五千万人，全球总伤亡人数估计达七千万至八千五百万人。\n"
            "战争并无真正的荣耀，唯有无尽的废墟，和生者漫长的救赎与反思。\n\n"
            "感谢通关本游戏。愿世界永无战争。");
        addNode(n);
    }

    // ========================================================
    // 节点 21: 结局 — 隐蔽等待 (Ending)
    // ========================================================
    {
        StoryNode n;
        n.id            = QStringLiteral("be_wait_end");
        n.locationTitle = QStringLiteral("柏林战役 — 结束（隐蔽等待）");
        n.musicKey      = QStringLiteral("berlin_end");
        n.type          = NodeType::Ending;
        n.isVictory     = true;
        n.text =
            QStringLiteral("【战役结束 — 存活】\n\n"
            "你把军装脱掉，换上了从废墟里捡来的平民旧大衣，把枪支和弹药深埋在了瓦砾堆下。\n"
            "你躲在一处挤满了妇女、儿童和老人的公寓地窖里。地窖里阴暗潮湿，只有微弱的蜡烛光照亮人们恐惧的脸庞。\n\n"
            "1945年5月2日，防空防空掩体外传来了高音喇叭的声音：魏德林将军已下令柏林守军全部停火。\n"
            "1945年5月8日，德国宣布无条件投降。当你走出地窖，刺眼的阳光让你几乎无法睁开眼睛。\n"
            "街上已经没有了枪声，只有苏联红军的卡车轰鸣着驶过，以及在瓦砾中清理杂物的妇女们。你虽然活了下来，但往后余生，那些在战场上死去的面孔、那个拿铁拳的孩子、那场疯狂的梦魇，将永远伴随着你。\n\n"
            "────────────────\n\n"
            "第二次世界大战欧洲战场以纳粹德国的无条件投降而告终。\n"
            "这场战争造成了人类历史上空前的浩劫，仅欧洲战场的死亡人数就高达四千万至五千万人，全球总伤亡人数估计达七千万至八千五百万人。\n"
            "战争并无真正的荣耀，唯有无尽的废墟，和生者漫长的救赎与反思。\n\n"
            "感谢通关本游戏。愿世界永无战争。");
        addNode(n);
    }

    // ========================================================
    // 节点 22: 结局 — 阵亡 (Ending)
    // ========================================================
    {
        StoryNode n;
        n.id            = QStringLiteral("be_captured");
        n.locationTitle = QStringLiteral("柏林战役 — 结束（突围失败）");
        n.musicKey      = QStringLiteral("defeat_theme");
        n.type          = NodeType::Ending;
        n.isDefeat      = true;
        n.text =
            QStringLiteral("【战役结束 — 阵亡】\n\n"
            "在向西突围的过程中，你们的小分队遭遇了红军装甲火力的正面拦截。\n"
            "苏军坦克的车载机枪在大街上扫射，子弹将周围的水泥柱打成碎末。你身边的同伴一个接一个地倒在血泊中。\n\n"
            "当你试图穿过十字路口时，一颗重机枪子弹击穿了你的胸膛。\n"
            "你无力地倒在冰冷的柏林街道上，枪声和呐喊声在你的耳边逐渐远去，视线渐渐模糊。\n\n"
            "在生命的最后一刻，你脑海里浮现的不是元首，也不是帝国，而是德意志故乡那一望无际的麦田，和母亲在夕阳下的微笑。\n"
            "这宏大的帝国废墟，最终成为了你毫无意义的坟墓。");
        addNode(n);
    }
}
