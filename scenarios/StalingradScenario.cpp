#include "StalingradScenario.h"
#include "../core/StoryNode.h"

QString StalingradScenario::startNodeId(PlayerClass cls) const {
    if (cls == PlayerClass::TankCrew)
        return QStringLiteral("st_briefing_tank");
    return QStringLiteral("st_briefing_inf");
}

void StalingradScenario::initialize() {

    // ========================================================
    // 节点 1: 步兵简报
    // ========================================================
    {
        StoryNode n;
        n.id            = QStringLiteral("st_briefing_inf");
        n.locationTitle = QStringLiteral("1942年8月 · 伏尔加河西岸，集结地");
        n.musicKey      = QStringLiteral("stalingrad");
        n.type          = NodeType::Narrative;
        n.text =
            QStringLiteral("斯大林格勒。\n\n"
            "这座城市以那个名字命名——这本身就让你感到某种奇异的沉重。\n\n"
            "你是第六集团军第71步兵师的一名士兵。从高地望去，"
            "城市的烟柱遮住了伏尔加河，断裂的工厂烟囱像折断的手指指向天空。\n\n"
            "「拿下这座城市，」上尉说，「就意味着切断苏联的南部防线。」\n\n"
            "他没有说这意味着什么代价。\n\n"
            "城市里的枪声已经响了一个星期。你需要冲进那片废墟。");
        n.nextNodeId = QStringLiteral("st_approach");
        addNode(n);
    }

    // ========================================================
    // 节点 2: 坦克兵简报
    // ========================================================
    {
        StoryNode n;
        n.id            = QStringLiteral("st_briefing_tank");
        n.locationTitle = QStringLiteral("1942年8月 · 斯大林格勒南翼，装甲集结");
        n.musicKey      = QStringLiteral("stalingrad");
        n.type          = NodeType::Narrative;
        n.text =
            QStringLiteral("坦克不适合在城市里作战。\n\n"
            "你知道这一点，每个装甲兵都知道这一点，但命令就是命令。\n\n"
            "你的四号坦克停在斯大林格勒郊外的草原上，炮管指着那座燃烧的城市。\n"
            "战场上，德国突击炮的残骸和苏军T-34的残骸以奇异的方式纠缠在一起——\n"
            "好像钢铁也会以这种方式死去。\n\n"
            "「城市里的苏军，」无线电里的命令，「必须被消灭。进城。」\n\n"
            "你启动发动机，坦克轰鸣着向废墟驶去。");
        n.nextNodeId = QStringLiteral("st_approach");
        addNode(n);
    }

    // ========================================================
    // 节点 3: 推进（战斗）
    // ========================================================
    {
        StoryNode n;
        n.id            = QStringLiteral("st_approach");
        n.locationTitle = QStringLiteral("1942年8月23日 · 斯大林格勒郊外");
        n.musicKey      = QStringLiteral("stalingrad");
        n.type          = NodeType::Choice;
        n.text =
            QStringLiteral("从集结地到城市边缘，只有三公里，但那三公里像三百公里一样长。\n\n"
            "苏军的炮火不间断地覆盖着推进路线，地面上是弹坑、碎石和倒在各处的尸体——\n"
            "分不清那些是德国人还是苏联人，甚至有平民的衣物。\n\n"
            "你需要突过这片死亡地带。");

        Choice c1;
        c1.text             = QStringLiteral("【匍匐】利用弹坑为掩体，匍匐推进");
        c1.isCombat         = true;
        c1.combatThreshold  = 50;
        c1.bonusClasses     = {PlayerClass::Infantry};
        c1.successNodeId    = QStringLiteral("st_city_edge");
        c1.failureNodeId    = QStringLiteral("st_approach_fail");
        c1.failHpDelta      = -25;
        c1.failMoraleDelta  = -10;

        Choice c2;
        c2.text             = QStringLiteral("【装甲掩护】跟在坦克后面，利用装甲掩护冲刺");
        c2.isCombat         = true;
        c2.combatThreshold  = 45;
        c2.bonusClasses     = {PlayerClass::TankCrew};
        c2.successNodeId    = QStringLiteral("st_city_edge");
        c2.failureNodeId    = QStringLiteral("st_approach_tank");
        c2.failHpDelta      = -30;
        c2.failMoraleDelta  = -10;

        n.choices = {c1, c2};
        addNode(n);
    }

    // ========================================================
    // 节点 4: 推进伤亡
    // ========================================================
    {
        StoryNode n;
        n.id            = QStringLiteral("st_approach_fail");
        n.locationTitle = QStringLiteral("1942年8月 · 斯大林格勒郊外");
        n.musicKey      = QStringLiteral("stalingrad");
        n.type          = NodeType::Narrative;
        n.text =
            QStringLiteral("炮弹在你旁边的弹坑里炸开，气浪把你掀翻在地。\n\n"
            "你旁边的士兵——格哈德，从汉诺威来的，结婚才半年——"
            "肠子从腹部流出来，他叫着你的名字，叫着他妻子的名字。\n\n"
            "你没有停下来，你无法停下来。\n\n"
            "他的声音在你身后越来越弱，最后被炮声淹没。\n\n"
            "你不知道他后来有没有得到救治。\n"
            "你选择了不去知道。");
        n.nextNodeId = QStringLiteral("st_city_edge");
        addNode(n);
    }

    // ========================================================
    // 节点 5: 坦克损毁
    // ========================================================
    {
        StoryNode n;
        n.id            = QStringLiteral("st_approach_tank");
        n.locationTitle = QStringLiteral("1942年8月 · 斯大林格勒郊外");
        n.musicKey      = QStringLiteral("stalingrad");
        n.type          = NodeType::Narrative;
        n.text =
            QStringLiteral("反坦克炮的穿甲弹击中了你的履带。\n\n"
            "坦克猛地停下，驾驶员舱的门被气浪焊死了，你用力踢了三脚才踢开。\n\n"
            "你爬出来的时候，看见炮手海因茨趴在坦克外面，\n"
            "他的后背被弹片划开了一道口子，血流得很快。\n"
            "装填手已经没有了。\n\n"
            "你们三个幸存者徒步冲进了城市。");
        n.nextNodeId = QStringLiteral("st_city_edge");
        addNode(n);
    }

    // ========================================================
    // 节点 6: 城市边缘（选择）
    // ========================================================
    {
        StoryNode n;
        n.id            = QStringLiteral("st_city_edge");
        n.locationTitle = QStringLiteral("1942年9月 · 斯大林格勒城区边缘");
        n.musicKey      = QStringLiteral("stalingrad");
        n.type          = NodeType::Choice;
        n.text =
            QStringLiteral("废墟。\n\n"
            "曾经是斯大林格勒繁华市区的地方，现在是一片碎石的海洋。\n"
            "楼房只剩下骨架，每一层楼都可能有枪口在等待，\n"
            "每一个窗洞都是一张嘴，不知道里面是生还是死。\n\n"
            "「每向前一米都要付出代价。」步兵战术手册上是这么说的。\n"
            "你来了斯大林格勒后，才明白这句话的重量。\n\n"
            "你的部队要拿下前方的街区，通往拖拉机厂的方向。");

        Choice c1;
        c1.text             = QStringLiteral("【正面】逐楼逐室清扫，直接突进");
        c1.isCombat         = true;
        c1.combatThreshold  = 60;
        c1.bonusClasses     = {PlayerClass::Infantry};
        c1.successNodeId    = QStringLiteral("st_factory_district");
        c1.failureNodeId    = QStringLiteral("st_pinned");
        c1.failHpDelta      = -30;
        c1.failMoraleDelta  = -15;

        Choice c2;
        c2.text             = QStringLiteral("【迂回】穿过下水道，从侧翼包抄苏军阵地");
        c2.isCombat         = true;
        c2.combatThreshold  = 45;
        c2.bonusClasses     = {PlayerClass::Infantry};
        c2.successNodeId    = QStringLiteral("st_factory_district");
        c2.failureNodeId    = QStringLiteral("st_ambush");
        c2.failHpDelta      = -20;
        c2.failMoraleDelta  = -10;

        n.choices = {c1, c2};
        addNode(n);
    }

    // ========================================================
    // 节点 7: 被压制
    // ========================================================
    {
        StoryNode n;
        n.id            = QStringLiteral("st_pinned");
        n.locationTitle = QStringLiteral("1942年9月 · 斯大林格勒废墟");
        n.musicKey      = QStringLiteral("stalingrad");
        n.type          = NodeType::Narrative;
        n.text =
            QStringLiteral("狙击手。\n\n"
            "你趴在一堵断墙后面，不敢动弹。子弹每隔几秒就打在墙头，\n"
            "把砖屑弹到你的脸上。\n\n"
            "你旁边的士兵微微抬起头，想看清楚狙击手的位置——\n"
            "然后他缓缓滑倒，靠在你的肩膀上。子弹从他的太阳穴穿进去。\n\n"
            "你抱着他等了两个小时，直到支援部队赶来，用炮弹炸掉了狙击手的位置。\n\n"
            "那个苏联狙击手可能也才十几岁，可能是个女孩。\n"
            "你无从知晓。");
        n.nextNodeId = QStringLiteral("st_factory_district");
        addNode(n);
    }

    // ========================================================
    // 节点 8: 伏击
    // ========================================================
    {
        StoryNode n;
        n.id            = QStringLiteral("st_ambush");
        n.locationTitle = QStringLiteral("1942年9月 · 斯大林格勒地下");
        n.musicKey      = QStringLiteral("stalingrad");
        n.type          = NodeType::Narrative;
        n.text =
            QStringLiteral("下水道里很暗，很臭，水深到膝盖。\n\n"
            "苏军在等着你们。手榴弹从暗处飞来，爆炸在密闭空间里撕裂性的响声让你短暂失聪。\n\n"
            "你失去了两名同伴。\n\n"
            "当你们最终爬出下水道，重新站在废墟之中，\n"
            "你发现你的手在颤抖，不是因为寒冷，而是因为某种你说不清楚的东西。");
        n.nextNodeId = QStringLiteral("st_factory_district");
        addNode(n);
    }

    // ========================================================
    // 节点 9: 拖拉机厂（选择）
    // ========================================================
    {
        StoryNode n;
        n.id            = QStringLiteral("st_factory_district");
        n.locationTitle = QStringLiteral("1942年10月 · 斯大林格勒拖拉机厂");
        n.musicKey      = QStringLiteral("stalingrad");
        n.type          = NodeType::Choice;
        n.text =
            QStringLiteral("斯大林格勒拖拉机厂。这里生产坦克，现在也是这场战争里最残酷的角斗场。\n\n"
            "苏军已经控制了工厂的上半部分，你们控制着下半部分。\n"
            "双方之间的分界线有时只是一堵墙，一层楼。\n"
            "同一栋建筑里，楼上是苏联红军，楼下是德国国防军，\n"
            "他们在砖墙和楼板里互相厮杀。\n\n"
            "这不是战争——这是屠杀。");

        Choice c1;
        c1.text             = QStringLiteral("【强攻】直接攻击苏军控制区，逐层清扫");
        c1.isCombat         = true;
        c1.combatThreshold  = 65;
        c1.bonusClasses     = {PlayerClass::Infantry};
        c1.successNodeId    = QStringLiteral("st_factory_taken");
        c1.failureNodeId    = QStringLiteral("st_fire_trap");
        c1.failHpDelta      = -35;
        c1.failMoraleDelta  = -20;

        Choice c2;
        c2.text             = QStringLiteral("【炮火】呼叫炮兵轰炸，用炮火替代步兵的血肉");
        c2.nextNodeId       = QStringLiteral("st_artillery");
        c2.moraleDelta      = -15;
        c2.grantedFlags     = {QStringLiteral("called_artillery")};

        Choice c3;
        c3.text             = QStringLiteral("【迂回】绕过工厂，向伏尔加河方向继续推进");
        c3.nextNodeId       = QStringLiteral("st_bypass");
        c3.moraleDelta      = -20;

        n.choices = {c1, c2, c3};
        addNode(n);
    }

    // ========================================================
    // 节点 10: 炮火覆盖
    // ========================================================
    {
        StoryNode n;
        n.id            = QStringLiteral("st_artillery");
        n.locationTitle = QStringLiteral("1942年10月 · 斯大林格勒拖拉机厂");
        n.musicKey      = QStringLiteral("stalingrad");
        n.type          = NodeType::Narrative;
        n.text =
            QStringLiteral("炮弹将整个工厂建筑群炸成了废墟中的废墟。\n\n"
            "你站在远处，看着那些楼层一层一层地坍塌，感到某种奇异的虚空。\n\n"
            "废墟下面，你无法知道里面是否还有人——"
            "苏联士兵，也许，工厂的工人，也许，还没来得及撤离的平民，也许。\n\n"
            "你没有去查。\n\n"
            "「工厂区清除，继续推进。」");
        n.nextNodeId = QStringLiteral("st_factory_taken");
        addNode(n);
    }

    // ========================================================
    // 节点 11: 迂回绕过
    // ========================================================
    {
        StoryNode n;
        n.id            = QStringLiteral("st_bypass");
        n.locationTitle = QStringLiteral("1942年10月 · 斯大林格勒街道");
        n.musicKey      = QStringLiteral("stalingrad");
        n.type          = NodeType::Narrative;
        n.text =
            QStringLiteral("你绕开了工厂，向伏尔加河方向继续推进。\n\n"
            "身后，工厂方向的枪声又响了，更激烈了——你留在那里的战友们还在里面。\n\n"
            "你没有回头。你告诉自己，那是战术需要，不是遗弃。\n\n"
            "但那枪声在你脑子里留了很久。");
        n.nextNodeId = QStringLiteral("st_factory_taken");
        addNode(n);
    }

    // ========================================================
    // 节点 12: 火场陷阱
    // ========================================================
    {
        StoryNode n;
        n.id            = QStringLiteral("st_fire_trap");
        n.locationTitle = QStringLiteral("1942年10月 · 拖拉机厂内部");
        n.musicKey      = QStringLiteral("stalingrad");
        n.type          = NodeType::Narrative;
        n.text =
            QStringLiteral("苏军点燃了储油罐。\n\n"
            "火焰顺着工厂的地沟蔓延，出口被火封死。\n"
            "你和三名同伴被困在一个小隔间里，烟很快变得密不透风。\n\n"
            "「找出口！」你带着他们摸索，最终撞开了一扇锁死的铁门，\n"
            "滚出厂房外，落在废墟里，浑身都是烟灰。\n\n"
            "身后，还有人的哭喊声。你们救不了更多人了。");
        n.nextNodeId = QStringLiteral("st_factory_taken");
        addNode(n);
    }

    // ========================================================
    // 节点 13: 工厂占领（选择）
    // ========================================================
    {
        StoryNode n;
        n.id            = QStringLiteral("st_factory_taken");
        n.locationTitle = QStringLiteral("1942年11月 · 斯大林格勒，伏尔加河边");
        n.musicKey      = QStringLiteral("stalingrad");
        n.type          = NodeType::Choice;
        n.text =
            QStringLiteral("工厂区落入德军手中。伏尔加河就在前方，近得可以看到对岸。\n\n"
            "但苏联的增援源源不断地渡河而来。每击毙一个苏联士兵，\n"
            "就会有两个从伏尔加河对岸补充进来。\n\n"
            "你看着浩荡的伏尔加河，感到一种无边的疲惫。\n\n"
            "「继续强攻还是巩固阵地？」上级等待你的建议。");

        Choice c1;
        c1.text             = QStringLiteral("【继续】强攻伏尔加河岸，彻底封锁苏军补给");
        c1.isCombat         = true;
        c1.combatThreshold  = 70;
        c1.bonusClasses     = {};
        c1.successNodeId    = QStringLiteral("st_volga_reach");
        c1.failureNodeId    = QStringLiteral("st_heavy_loss");
        c1.failHpDelta      = -40;
        c1.failMoraleDelta  = -15;

        Choice c2;
        c2.text             = QStringLiteral("【巩固】原地防守，等待后方增援");
        c2.nextNodeId       = QStringLiteral("st_winter_comes");

        n.choices = {c1, c2};
        addNode(n);
    }

    // ========================================================
    // 节点 14: 重大伤亡
    // ========================================================
    {
        StoryNode n;
        n.id            = QStringLiteral("st_heavy_loss");
        n.locationTitle = QStringLiteral("1942年11月 · 斯大林格勒废墟");
        n.musicKey      = QStringLiteral("stalingrad");
        n.type          = NodeType::Narrative;
        n.text =
            QStringLiteral("连队伤亡已经超过七成。\n\n"
            "副排长的头颅被弹片削去了一半——你看见时，他还站在那里，\n"
            "然后才慢慢倒下，就像一棵被锯断的树。\n\n"
            "你在废墟里捡到了他的钢盔。钢盔里有一张被折叠过很多次的照片——\n"
            "一个女人抱着一个小女孩，背景是某个德国小城市的广场。\n\n"
            "你把照片装进口袋。你不知道为什么，但你把它装进去了。");
        n.nextNodeId = QStringLiteral("st_winter_comes");
        addNode(n);
    }

    // ========================================================
    // 节点 15: 伏尔加河岸
    // ========================================================
    {
        StoryNode n;
        n.id            = QStringLiteral("st_volga_reach");
        n.locationTitle = QStringLiteral("1942年11月 · 伏尔加河西岸");
        n.musicKey      = QStringLiteral("stalingrad");
        n.type          = NodeType::Narrative;
        n.text =
            QStringLiteral("你冲到了伏尔加河岸。\n\n"
            "河面很宽，冰冷的河水在晨光下发着灰蓝色的光。\n"
            "对岸，是苏联，无穷尽的苏联。\n\n"
            "你感到一阵眩晕，不是因为受伤，而是因为突然明白了一件事：\n"
            "就算拿下斯大林格勒，拿下整个苏联又如何？\n"
            "这片土地太大了，大到可以把所有的战争都淹没。\n\n"
            "然后，1942年11月19日，天王星行动。");
        n.nextNodeId = QStringLiteral("st_encirclement");
        addNode(n);
    }

    // ========================================================
    // 节点 16: 冬天来了（选择）
    // ========================================================
    {
        StoryNode n;
        n.id            = QStringLiteral("st_winter_comes");
        n.locationTitle = QStringLiteral("1942年11月 · 斯大林格勒，冬日");
        n.musicKey      = QStringLiteral("stalingrad_winter");
        n.type          = NodeType::Choice;
        n.text =
            QStringLiteral("冬天来了。\n\n"
            "来自西伯利亚的寒流把斯大林格勒变成了冰的地狱。零下三十度，零下四十度。\n\n"
            "你们没有足够的冬装——后勤以为到冬天之前就能拿下城市，所以没有备足。\n\n"
            "士兵们用死去的战友的大衣，用苏联平民的破布，包裹自己。\n"
            "每天早晨都有人在战壕里冻死，不是被打死，是冻死。\n\n"
            "食物开始匮乏。");

        Choice c1;
        c1.text             = QStringLiteral("【搜寻】带人搜寻周围废墟里的食物");
        c1.isCombat         = true;
        c1.combatThreshold  = 40;
        c1.bonusClasses     = {PlayerClass::Infantry};
        c1.successNodeId    = QStringLiteral("st_food_found");
        c1.failureNodeId    = QStringLiteral("st_starving");
        c1.failHpDelta      = -20;
        c1.failMoraleDelta  = -15;

        Choice c2;
        c2.text             = QStringLiteral("【分享】将最后的口粮平均分给所有人");
        c2.nextNodeId       = QStringLiteral("st_share_ration");
        c2.moraleDelta      = 10;
        c2.hpDelta          = -10;
        c2.grantedFlags     = {QStringLiteral("shared_ration")};

        n.choices = {c1, c2};
        addNode(n);
    }

    // ========================================================
    // 节点 17: 找到食物
    // ========================================================
    {
        StoryNode n;
        n.id            = QStringLiteral("st_food_found");
        n.locationTitle = QStringLiteral("1942年12月 · 斯大林格勒废墟地窖");
        n.musicKey      = QStringLiteral("stalingrad_winter");
        n.type          = NodeType::Narrative;
        n.text =
            QStringLiteral("你们在一处废弃民居的地窖里找到了腌白菜和一袋黑麦粉。\n\n"
            "地窖的角落里，一个玩具熊靠在墙边，一个棉布做的小熊，"
            "某个苏联孩子留下来的。\n\n"
            "你的士兵们把腌菜分了，吃得很快，没有人说话。\n\n"
            "出来的时候，你把那个玩具熊放回了原位。\n"
            "你不知道为什么这个举动让你喉咙发紧。");
        n.nextNodeId = QStringLiteral("st_encirclement");
        addNode(n);
    }

    // ========================================================
    // 节点 18: 饥饿
    // ========================================================
    {
        StoryNode n;
        n.id            = QStringLiteral("st_starving");
        n.locationTitle = QStringLiteral("1942年12月 · 斯大林格勒");
        n.musicKey      = QStringLiteral("stalingrad_winter");
        n.type          = NodeType::Narrative;
        n.text =
            QStringLiteral("三天没有进食。\n\n"
            "你的双手开始颤抖，不是因为寒冷，而是因为饥饿。\n\n"
            "有人开始吃皮带。有人煮了鞋底。\n"
            "一匹死掉的马被发现，二十分钟内被分光，包括骨头。\n\n"
            "你在战壕里看着天空，感觉自己的思维变得很奇怪，\n"
            "开始想一些乱七八糟的事情——妈妈做的土豆汤，圣诞节的蜡烛，\n"
            "以及所有那些你以为理所当然的、普通的、温暖的东西。");
        n.nextNodeId = QStringLiteral("st_encirclement");
        addNode(n);
    }

    // ========================================================
    // 节点 19: 分享口粮
    // ========================================================
    {
        StoryNode n;
        n.id            = QStringLiteral("st_share_ration");
        n.locationTitle = QStringLiteral("1942年12月 · 斯大林格勒战壕");
        n.musicKey      = QStringLiteral("stalingrad_winter");
        n.type          = NodeType::Narrative;
        n.text =
            QStringLiteral("你把最后的黑面包掰开，每个人分到大约一个手掌大小的一块。\n\n"
            "大家围坐在一起，没有人说话，只是吃。\n\n"
            "吃完之后，有人低声哼起了圣诞歌。\n"
            "那是《平安夜》，德语版的。\n\n"
            "在这个世界上最残酷的地方，这一刻如此不真实，\n"
            "又如此清晰。\n\n"
            "然后1942年11月19日，苏军的大炮开始轰鸣。");
        n.nextNodeId = QStringLiteral("st_encirclement");
        addNode(n);
    }

    // ========================================================
    // 节点 20: 包围圈（选择）
    // ========================================================
    {
        StoryNode n;
        n.id            = QStringLiteral("st_encirclement");
        n.locationTitle = QStringLiteral("1942年11月19日 · 天王星行动");
        n.musicKey      = QStringLiteral("stalingrad_winter");
        n.type          = NodeType::Choice;
        n.text =
            QStringLiteral("1942年11月19日，苏联发动天王星行动。\n\n"
            "消息在几个小时内传遍所有阵地：我们被包围了。\n\n"
            "两股苏联装甲力量在卡拉奇合围，包围圈里有三十万德军和轴心国军队。\n"
            "包围圈的直径一百公里，周长是一道铁一样的苏联防线。\n\n"
            "「最高统帅部命令坚守，解救行动正在准备。」无线电里的声音说。\n\n"
            "你看着你的士兵们，他们比你更清楚这意味着什么。");

        Choice c1;
        c1.text             = QStringLiteral("【突围】立即尝试突破包围圈，向西撤退");
        c1.isCombat         = true;
        c1.combatThreshold  = 65;
        c1.bonusClasses     = {};
        c1.successNodeId    = QStringLiteral("st_breakout_success");
        c1.failureNodeId    = QStringLiteral("st_breakout_fail");
        c1.failHpDelta      = -35;
        c1.failMoraleDelta  = -25;

        Choice c2;
        c2.text             = QStringLiteral("【坚守】遵守命令，等待解救行动");
        c2.nextNodeId       = QStringLiteral("st_wait_rescue");

        n.choices = {c1, c2};
        addNode(n);
    }

    // ========================================================
    // 节点 21: 等待解救
    // ========================================================
    {
        StoryNode n;
        n.id            = QStringLiteral("st_wait_rescue");
        n.locationTitle = QStringLiteral("1942年12月 · 斯大林格勒包围圈");
        n.musicKey      = QStringLiteral("stalingrad_winter");
        n.type          = NodeType::Narrative;
        n.text =
            QStringLiteral("曼施坦因的解救行动开始了，然后停止了，然后失败了。\n\n"
            "包围圈越收越紧。空运的物资——每天几百吨，但需要几千吨——"
            "根本填不满三十万人的缺口。\n\n"
            "圣诞夜。某个战壕里有人在唱《平安夜》，断断续续的，\n"
            "然后被冻住了，或者停下来了，再也没有继续。\n\n"
            "1943年1月，元首的命令：不得投降，不得后撤，死而后已。\n\n"
            "你握紧步枪，感觉枪托上的木头已经被你摸出了油脂的光泽。");
        n.nextNodeId = QStringLiteral("st_final_days");
        addNode(n);
    }

    // ========================================================
    // 节点 22: 突围成功
    // ========================================================
    {
        StoryNode n;
        n.id            = QStringLiteral("st_breakout_success");
        n.locationTitle = QStringLiteral("1943年1月 · 包围圈外");
        n.musicKey      = QStringLiteral("stalingrad_winter");
        n.type          = NodeType::Narrative;
        n.text =
            QStringLiteral("你们冲出去了。\n\n"
            "七个人，从三十人的小分队里剩下的七个人。\n\n"
            "你在雪地里奔跑，枪声从四面八方传来，\n"
            "但你们的脚步越来越远，越来越远，直到那座燃烧的城市的轮廓消失在夜色和雪雾中。\n\n"
            "当你回望斯大林格勒的方向，那里的天空是红色的。\n\n"
            "身后，三十万人仍然在那片红色的天空下。");
        n.nextNodeId = QStringLiteral("st_final_days");
        addNode(n);
    }

    // ========================================================
    // 节点 23: 突围失败
    // ========================================================
    {
        StoryNode n;
        n.id            = QStringLiteral("st_breakout_fail");
        n.locationTitle = QStringLiteral("1943年1月 · 包围圈");
        n.musicKey      = QStringLiteral("stalingrad_winter");
        n.type          = NodeType::Narrative;
        n.text =
            QStringLiteral("突围失败了。\n\n"
            "你们被打回来。雪地上倒满了人，分不清是冻死的还是中弹死的，\n"
            "因为冻死的人和中弹死的人看起来一样——都静止，都苍白，都被雪覆盖。\n\n"
            "你回到了原来的阵地，比出发时少了十三个人。\n\n"
            "「我们再试一次，」有人说。\n「不，」你说，「我们等。」\n"
            "「等什么？」\n\n"
            "你没有回答这个问题。");
        n.nextNodeId = QStringLiteral("st_final_days");
        addNode(n);
    }

    // ========================================================
    // 节点 24: 最后的日子（选择）
    // ========================================================
    {
        StoryNode n;
        n.id            = QStringLiteral("st_final_days");
        n.locationTitle = QStringLiteral("1943年2月2日 · 斯大林格勒");
        n.musicKey      = QStringLiteral("stalingrad_winter");
        n.type          = NodeType::Choice;
        n.text =
            QStringLiteral("1943年2月2日。\n\n"
            "保卢斯元帅投降了，带着他的参谋部走出地窖，向苏军投降。\n\n"
            "消息传遍了所有阵地。有人哭，有人怒骂，有人只是沉默地放下了枪。\n\n"
            "你的小分队还活着。枪声停了，但周围是无边无际的苏联士兵。\n\n"
            "这一刻，你终于可以做出属于自己的选择。");

        Choice c1;
        c1.text         = QStringLiteral("【放下武器】将步枪插在雪地里，走向苏军阵地");
        c1.nextNodeId   = QStringLiteral("st_victory");

        Choice c2;
        c2.text         = QStringLiteral("【继续战斗】为德意志战斗到最后一颗子弹");
        c2.isCombat     = true;
        c2.combatThreshold = 90;
        c2.successNodeId = QStringLiteral("st_last_stand");
        c2.failureNodeId = QStringLiteral("st_defeat");
        c2.failHpDelta   = -50;
        c2.failMoraleDelta = -30;

        n.choices = {c1, c2};
        addNode(n);
    }

    // ========================================================
    // 节点 25: 胜利 — 投降
    // ========================================================
    {
        StoryNode n;
        n.id            = QStringLiteral("st_victory");
        n.locationTitle = QStringLiteral("斯大林格勒战役 — 结束");
        n.musicKey      = QStringLiteral("stalingrad_end");
        n.type          = NodeType::Ending;
        n.isVictory     = true;
        n.text =
            QStringLiteral("【战役结束 — 存活】\n\n"
            "你把步枪插在雪地里，双手举起，向苏军阵地走去。\n\n"
            "对面走来的苏联士兵和你一样年轻，和你一样疲惫，和你一样瘦削，"
            "眼睛里是和你一样的某种虚空。\n\n"
            "没有人说话。战争，在这个小小的时刻，仿佛停止了。\n\n"
            "────────────────\n\n"
            "斯大林格勒战役是人类历史上最惨烈的城市战役。\n"
            "双方死亡、受伤或被俘人数：约一百九十五万至两百万人。\n\n"
            "第六集团军九十一名将领中有二十四名将领随保卢斯投降，\n"
            "普通士兵约九万一千人成为战俘——其中只有六千人活着回到了德国。\n\n"
            "这不是胜利，也不是失败。\n这是人类给自己开出的一张价目表，\n"
            "记录着一种文明能够对另一种文明，对自己，做到什么地步。\n\n"
            "下一章即将开启。");
        addNode(n);
    }

    // ========================================================
    // 节点 26: 最后的坚持
    // ========================================================
    {
        StoryNode n;
        n.id            = QStringLiteral("st_last_stand");
        n.locationTitle = QStringLiteral("斯大林格勒战役 — 结束");
        n.musicKey      = QStringLiteral("stalingrad_end");
        n.type          = NodeType::Ending;
        n.isVictory     = true;
        n.text =
            QStringLiteral("【战役结束 — 存活】\n\n"
            "你打完了最后一颗子弹，然后也没有更多的子弹了。\n\n"
            "苏联士兵走进来，看见你坐在那里，没有武器，"
            "只是靠着一堵断墙，已经精疲力竭到无法站起来。\n\n"
            "他们没有开枪。其中一个把一个半块的面包放到你面前。\n\n"
            "你吃了那块面包。\n\n"
            "后来你成了战俘。活了下来。\n\n"
            "斯大林格勒共有约两百万人在这场战役中死亡或失踪。\n"
            "你不是其中之一，但你不知道这是幸运还是不幸。\n\n"
            "下一章即将开启。");
        addNode(n);
    }

    // ========================================================
    // 节点 27: 失败
    // ========================================================
    {
        StoryNode n;
        n.id            = QStringLiteral("st_defeat");
        n.locationTitle = QStringLiteral("斯大林格勒战役 — 失败");
        n.musicKey      = QStringLiteral("defeat_theme");
        n.type          = NodeType::Ending;
        n.isDefeat      = true;
        n.text =
            QStringLiteral("【战役结束 — 阵亡】\n\n"
            "最后一颗子弹，不是打出去的，是打进来的。\n\n"
            "你倒在斯大林格勒的废墟里，雪很快覆盖了你。\n\n"
            "这座城市最终被双方合力摧毁，然后以同样的代价重建。\n\n"
            "你的名字，不会出现在任何地方。");
        addNode(n);
    }
}
