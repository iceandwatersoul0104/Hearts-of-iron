#include "FallGelbScenario.h"
#include "../core/StoryNode.h"

QString FallGelbScenario::startNodeId(PlayerClass cls) const {
    if (cls == PlayerClass::TankCrew)
        return QStringLiteral("fg_briefing_tank");
    return QStringLiteral("fg_briefing_inf");
}

void FallGelbScenario::initialize() {

    // ========================================================
    // 节点 1: 步兵简报
    // ========================================================
    {
        StoryNode n;
        n.id            = QStringLiteral("fg_briefing_inf");
        n.locationTitle = QStringLiteral("1940年5月10日凌晨3时 · 德比边境集结地");
        n.musicKey      = QStringLiteral("fallgelb");
        n.type          = NodeType::Narrative;
        n.text =
            QStringLiteral("黎明尚未到来，森林里充斥着发动机的低沉轰鸣与士兵们压低嗓音的交谈。\n\n"
            "你是第18步兵师的一名普通士兵，刚刚二十岁出头。两年前你还在巴伐利亚的农场帮父亲收割小麦。\n\n"
            "连长走到队伍前方，声音低沉：「今天，我们将翻越阿登山区，穿越比利时，直抵法国心脏。」\n\n"
            "他停顿了一下，目光扫过你们年轻的脸：「这将是历史性的一天。」\n\n"
            "没有人欢呼。黑暗中，每个人都沉默地检查着自己的步枪。\n\n"
            "命令下达：前进。");
        n.nextNodeId = QStringLiteral("fg_ardennes_march");
        addNode(n);
    }

    // ========================================================
    // 节点 2: 坦克兵简报
    // ========================================================
    {
        StoryNode n;
        n.id            = QStringLiteral("fg_briefing_tank");
        n.locationTitle = QStringLiteral("1940年5月10日凌晨3时 · 第19装甲军集结地");
        n.musicKey      = QStringLiteral("fallgelb");
        n.type          = NodeType::Narrative;
        n.text =
            QStringLiteral("你是古德里安将军麾下第19装甲军的一名坦克驾驶员，爬进四号坦克狭窄的驾驶舱时，\n"
            "金属舱壁仍带着夜晚的寒凉。\n\n"
            "「我们的任务，」无线电里传来上级的声音，「是以最快速度穿越阿登山区，"
            "在法军还没反应过来之前，冲到马斯河。」\n\n"
            "阿登——那片法国人和英国人认为坦克无法通过的密林。他们错了。\n\n"
            "你踩下油门，钢铁巨兽缓缓驶入黑暗的森林。");
        n.nextNodeId = QStringLiteral("fg_ardennes_march");
        addNode(n);
    }

    // ========================================================
    // 节点 3: 穿越阿登 (战斗)
    // ========================================================
    {
        StoryNode n;
        n.id            = QStringLiteral("fg_ardennes_march");
        n.locationTitle = QStringLiteral("1940年5月10日 · 阿登山区密林");
        n.musicKey      = QStringLiteral("fallgelb");
        n.type          = NodeType::Choice;
        n.text =
            QStringLiteral("阿登的山路比想象中更难走。崎岖的林间小道坑洼不平，车辆排成数公里长龙，"
            "每隔几分钟就会因前方的拥堵停下来等待。\n\n"
            "天色开始放亮。比利时边境的方向传来隐约的炮声——那是德国空军在轰炸。\n\n"
            "突然，前方传来枪声。一支比利时边境巡逻队发现了你们的先头部队。");

        Choice c1;
        c1.text             = QStringLiteral("【强攻】冲破巡逻队阻击，快速穿越");
        c1.isCombat         = true;
        c1.combatThreshold  = 50;
        c1.bonusClasses     = {PlayerClass::Infantry};
        c1.successNodeId    = QStringLiteral("fg_village_enter");
        c1.failureNodeId    = QStringLiteral("fg_ardennes_wounded");
        c1.failHpDelta      = -25;
        c1.failMoraleDelta  = -10;

        Choice c2;
        c2.text             = QStringLiteral("【迂回】绕开巡逻队，绕道穿越森林");
        c2.isCombat         = true;
        c2.combatThreshold  = 45;
        c2.bonusClasses     = {PlayerClass::TankCrew};
        c2.successNodeId    = QStringLiteral("fg_village_enter");
        c2.failureNodeId    = QStringLiteral("fg_ardennes_delay");
        c2.failHpDelta      = -10;
        c2.failMoraleDelta  = -15;

        n.choices = {c1, c2};
        addNode(n);
    }

    // ========================================================
    // 节点 4: 受伤战友
    // ========================================================
    {
        StoryNode n;
        n.id            = QStringLiteral("fg_ardennes_wounded");
        n.locationTitle = QStringLiteral("1940年5月10日 · 阿登山区");
        n.musicKey      = QStringLiteral("fallgelb");
        n.type          = NodeType::Narrative;
        n.text =
            QStringLiteral("子弹像冰雹一样打在树干上。你向前冲刺，旁边的弗里茨突然倒下——\n"
            "他大腿中弹，鲜血迅速染红了他的裤腿。\n\n"
            "「继续！」班长大喊。\n\n"
            "你没有停下。弗里茨的呼救声在你身后渐渐被枪声淹没。\n\n"
            "后来你再也没有见到他。");
        n.nextNodeId = QStringLiteral("fg_village_enter");
        addNode(n);
    }

    // ========================================================
    // 节点 5: 迂回延误
    // ========================================================
    {
        StoryNode n;
        n.id            = QStringLiteral("fg_ardennes_delay");
        n.locationTitle = QStringLiteral("1940年5月10日 · 阿登山区");
        n.musicKey      = QStringLiteral("fallgelb");
        n.type          = NodeType::Narrative;
        n.text =
            QStringLiteral("绕道让你们多花了三个小时。密林中，坦克履带一次次陷入泥泞，需要全员推动。\n\n"
            "当你们终于走出森林，天色已经大亮。前方的村庄飘起了炊烟——那里有人。\n\n"
            "普通的比利时村民，他们在自己家里，没想到战争会以这种方式来临。");
        n.nextNodeId = QStringLiteral("fg_village_enter");
        addNode(n);
    }

    // ========================================================
    // 节点 6: 进入比利时村庄 (选择)
    // ========================================================
    {
        StoryNode n;
        n.id            = QStringLiteral("fg_village_enter");
        n.locationTitle = QStringLiteral("1940年5月11日清晨 · 比利时小村");
        n.musicKey      = QStringLiteral("fallgelb_village");
        n.type          = NodeType::Choice;
        n.text =
            QStringLiteral("村庄叫做布拉尔，一个普通的比利时农村。砖瓦房子、小教堂、一口水井。\n\n"
            "但现在，广场上站满了惶恐的村民——老人、妇女、孩子。他们看着你们的军装，沉默着，"
            "眼中是无法掩盖的恐惧。\n\n"
            "一个老妇人抱着一只猫，那只猫在她怀里瑟瑟发抖。\n\n"
            "你的上级在等待你的部队快速通过，后面还有数公里的战线在等待。");

        Choice c1;
        c1.text         = QStringLiteral("【人道】让村民留在原地，下令禁止骚扰平民");
        c1.nextNodeId   = QStringLiteral("fg_village_pass");
        c1.moraleDelta  = 5;
        c1.grantedFlags = {QStringLiteral("spared_village")};

        Choice c2;
        c2.text         = QStringLiteral("【命令】驱赶村民离开，彻底搜查村庄");
        c2.nextNodeId   = QStringLiteral("fg_village_clear");
        c2.moraleDelta  = -20;

        n.choices = {c1, c2};
        addNode(n);
    }

    // ========================================================
    // 节点 7: 让村民留下
    // ========================================================
    {
        StoryNode n;
        n.id            = QStringLiteral("fg_village_pass");
        n.locationTitle = QStringLiteral("1940年5月11日 · 比利时小村");
        n.musicKey      = QStringLiteral("fallgelb_village");
        n.type          = NodeType::Narrative;
        n.text =
            QStringLiteral("你让村民留在原地，严令部下不得骚扰。\n\n"
            "经过广场时，那个老妇人向你投来一瞥——那不是感激，而是某种说不清楚的东西，"
            "也许是麻木，也许是祈祷你们快点离开。\n\n"
            "教堂的大钟突然开始鸣响——不知道是谁敲的，钟声在空旷的乡野里回荡，"
            "像是哀歌，又像是警报。\n\n"
            "你们快步穿过村子，向马斯河方向推进。");
        n.nextNodeId = QStringLiteral("fg_meuse_river");
        addNode(n);
    }

    // ========================================================
    // 节点 8: 驱赶村民
    // ========================================================
    {
        StoryNode n;
        n.id            = QStringLiteral("fg_village_clear");
        n.locationTitle = QStringLiteral("1940年5月11日 · 比利时小村");
        n.musicKey      = QStringLiteral("fallgelb_village");
        n.type          = NodeType::Narrative;
        n.text =
            QStringLiteral("士兵们开始用枪托驱赶村民。那个老妇人跌倒在地，猫从她怀中逃走，"
            "消失在小巷里。\n\n"
            "一个男孩，大概八九岁，死死抓着家门不肯走，直到有人粗暴地拉开他的手指。\n"
            "他哭喊着什么，你听不懂比利时语，但你能听懂哭声。\n\n"
            "搜查什么也没发现。当然，这只是一个普通的村子。\n\n"
            "你们走的时候，有人顺手打破了村里唯一一口钟。\n"
            "那一声破碎声，你很多年后还能听见。");
        n.nextNodeId = QStringLiteral("fg_meuse_river");
        addNode(n);
    }

    // ========================================================
    // 节点 9: 马斯河渡口 (战斗)
    // ========================================================
    {
        StoryNode n;
        n.id            = QStringLiteral("fg_meuse_river");
        n.locationTitle = QStringLiteral("1940年5月13日 · 马斯河渡口");
        n.musicKey      = QStringLiteral("fallgelb_battle");
        n.type          = NodeType::Choice;
        n.text =
            QStringLiteral("马斯河对岸，法军的机枪已经封锁了整个河面。\n\n"
            "河水是深绿色的，平静而美丽，与对岸的炮火形成荒诞的对比。\n\n"
            "工兵们抬着橡皮艇向河边冲去。前面几艘还没划到河中心，就已经被打成了筛子。\n"
            "士兵们的尸体顺流而下，染红了水面。\n\n"
            "你的连长转向你，眼神里有你从未在他脸上看到过的东西——也许是恐惧。\n"
            "「谁愿意第一批渡河？」");

        Choice c1;
        c1.text             = QStringLiteral("【主动】自愿加入第一批渡河突击队");
        c1.isCombat         = true;
        c1.combatThreshold  = 55;
        c1.bonusClasses     = {PlayerClass::Infantry};
        c1.successNodeId    = QStringLiteral("fg_meuse_crossed");
        c1.failureNodeId    = QStringLiteral("fg_meuse_hit");
        c1.failHpDelta      = -30;
        c1.failMoraleDelta  = -15;

        Choice c2;
        c2.text             = QStringLiteral("【等待】等第一批渡过后，随第二批跟进");
        c2.isCombat         = true;
        c2.combatThreshold  = 40;
        c2.successNodeId    = QStringLiteral("fg_meuse_crossed");
        c2.failureNodeId    = QStringLiteral("fg_meuse_sink");
        c2.failHpDelta      = -20;
        c2.failMoraleDelta  = -20;

        n.choices = {c1, c2};
        addNode(n);
    }

    // ========================================================
    // 节点 10: 渡河成功
    // ========================================================
    {
        StoryNode n;
        n.id            = QStringLiteral("fg_meuse_crossed");
        n.locationTitle = QStringLiteral("1940年5月13日 · 马斯河对岸");
        n.musicKey      = QStringLiteral("fallgelb_battle");
        n.type          = NodeType::Narrative;
        n.text =
            QStringLiteral("橡皮艇触岸的瞬间，你几乎是跌滚上了岸。法军的枪声从你耳边掠过，\n"
            "你听见身后有人落水，再也没有上来。\n\n"
            "对岸的工事已经出现。斯图卡俯冲轰炸机从头顶呼啸而过，"
            "法军阵地腾起巨大的火球。\n\n"
            "你冲进一处弹坑，喘着气。弹坑里有一具法国士兵的遗体，"
            "他的手里还握着一张照片——一个女人和两个孩子的合影。\n\n"
            "你没有时间去想太多。继续前进。");
        n.nextNodeId = QStringLiteral("fg_france_border");
        addNode(n);
    }

    // ========================================================
    // 节点 11: 渡河被击中
    // ========================================================
    {
        StoryNode n;
        n.id            = QStringLiteral("fg_meuse_hit");
        n.locationTitle = QStringLiteral("1940年5月13日 · 马斯河");
        n.musicKey      = QStringLiteral("fallgelb_battle");
        n.type          = NodeType::Narrative;
        n.text =
            QStringLiteral("橡皮艇刚离岸，机枪的弹雨就打来了。你的腿被擦伤，鲜血浸透了裤腿。\n\n"
            "艇上的人大喊大叫，有人跳入河中，有人俯身划桨。\n"
            "你不知道自己是怎么活着到达对岸的。\n\n"
            "上岸后，你才感觉到腿上的疼痛。一块弹片穿透了你的小腿。\n\n"
            "「能走吗？」卫生员问你。\n「能。」你咬着牙说。");
        n.nextNodeId = QStringLiteral("fg_france_border");
        addNode(n);
    }

    // ========================================================
    // 节点 12: 艇沉没
    // ========================================================
    {
        StoryNode n;
        n.id            = QStringLiteral("fg_meuse_sink");
        n.locationTitle = QStringLiteral("1940年5月13日 · 马斯河");
        n.musicKey      = QStringLiteral("fallgelb_battle");
        n.type          = NodeType::Narrative;
        n.text =
            QStringLiteral("橡皮艇在河中心被机枪打穿了。冰冷的河水灌进来，人们纷纷跳水。\n\n"
            "你会游泳。你努力向对岸游去，但盔甲和装备让你沉得厉害。\n"
            "子弹在水中划出白色的轨迹，像鱼，从你身边穿过。\n\n"
            "你不知道是用了多长时间，才爬上了对岸的泥土。\n"
            "你的旁边，三具同伴的尸体被水冲来。你认出了其中一个——"
            "他叫汉斯，来自慕尼黑，家里有四个弟弟妹妹。");
        n.nextNodeId = QStringLiteral("fg_france_border");
        addNode(n);
    }

    // ========================================================
    // 节点 13: 进入法国 (选择)
    // ========================================================
    {
        StoryNode n;
        n.id            = QStringLiteral("fg_france_border");
        n.locationTitle = QStringLiteral("1940年5月15日 · 法国色当附近");
        n.musicKey      = QStringLiteral("fallgelb_battle");
        n.type          = NodeType::Choice;
        n.text =
            QStringLiteral("你们突破了马斯河防线。法国的土地在你脚下展开——绿色的农田，\n"
            "蜿蜒的公路，远处村庄的尖顶。\n\n"
            "但前方的公路上，一队法国士兵正在仓皇设置防线。\n"
            "他们看上去和你们一样年轻，一样惶恐。\n\n"
            "他们在匆忙布置路障，架设机枪。你们还有时间做出选择。");

        Choice c1;
        c1.text             = QStringLiteral("【强攻】正面突击，打垮法军防线");
        c1.isCombat         = true;
        c1.combatThreshold  = 60;
        c1.bonusClasses     = {PlayerClass::TankCrew};
        c1.successNodeId    = QStringLiteral("fg_french_line_broken");
        c1.failureNodeId    = QStringLiteral("fg_french_line_pinned");
        c1.failHpDelta      = -30;
        c1.failMoraleDelta  = -10;

        Choice c2;
        c2.text             = QStringLiteral("【喊话】用德语喊话劝降，给他们投降的机会");
        c2.isCombat         = true;
        c2.combatThreshold  = 40;
        c2.successNodeId    = QStringLiteral("fg_french_surrender");
        c2.failureNodeId    = QStringLiteral("fg_french_line_pinned");
        c2.failHpDelta      = -15;
        c2.failMoraleDelta  = -15;

        n.choices = {c1, c2};
        addNode(n);
    }

    // ========================================================
    // 节点 14: 法军防线被突破
    // ========================================================
    {
        StoryNode n;
        n.id            = QStringLiteral("fg_french_line_broken");
        n.locationTitle = QStringLiteral("1940年5月15日 · 法国农村公路");
        n.musicKey      = QStringLiteral("fallgelb_battle");
        n.type          = NodeType::Narrative;
        n.text =
            QStringLiteral("炮弹、手榴弹，然后是刺刀。\n\n"
            "战斗结束得很快。但「结束」这个词掩盖了太多东西。\n\n"
            "公路上躺着七八具法军士兵的遗体，他们的制服和你的一样是布料缝制的，"
            "一样会被子弹穿透，一样会流血。\n\n"
            "你走过一个倒下的法国少尉，他的眼睛还睁着，嘴角有血泡，\n"
            "口袋里的照片正在被风吹走——两张，三张……那是他的家人。\n\n"
            "没有人去捡那些照片。");
        n.nextNodeId = QStringLiteral("fg_civilian_road");
        addNode(n);
    }

    // ========================================================
    // 节点 15: 法军投降
    // ========================================================
    {
        StoryNode n;
        n.id            = QStringLiteral("fg_french_surrender");
        n.locationTitle = QStringLiteral("1940年5月15日 · 法国农村");
        n.musicKey      = QStringLiteral("fallgelb_village");
        n.type          = NodeType::Narrative;
        n.text =
            QStringLiteral("「不要开枪！」\n\n"
            "沉默，然后是缓缓举起的双手。十一个法国士兵走出来，\n"
            "他们的脸上写满了疲惫与如释重负。\n\n"
            "其中一个说了很长一段法语，你不懂，但翻译凑过来：\n"
            "「他说，他在前线已经三天没吃东西了。他问你们是不是会枪毙俘虏。」\n\n"
            "你让人给了他们一些面包。他在哭，他试图藏住，但没能成功。");
        n.nextNodeId = QStringLiteral("fg_civilian_road");
        addNode(n);
    }

    // ========================================================
    // 节点 16: 压制在战壕
    // ========================================================
    {
        StoryNode n;
        n.id            = QStringLiteral("fg_french_line_pinned");
        n.locationTitle = QStringLiteral("1940年5月15日 · 法国农村公路");
        n.musicKey      = QStringLiteral("fallgelb_battle");
        n.type          = NodeType::Narrative;
        n.text =
            QStringLiteral("法军的机枪把你们死死压在路边的沟里。\n\n"
            "你旁边的埃里希探头看了一眼，脑袋后仰，再也没有动。\n"
            "你伸手摸了摸他的脖子——没有脉搏。只有一个弹孔，整齐得像一个句号。\n\n"
            "火炮支援终于在两小时后到来，法军阵地变成了废墟。\n\n"
            "你爬出来，感觉自己的腿像灌了铅。那个弹孔的位置，你记了很多年。");
        n.nextNodeId = QStringLiteral("fg_civilian_road");
        addNode(n);
    }

    // ========================================================
    // 节点 17: 难民公路
    // ========================================================
    {
        StoryNode n;
        n.id            = QStringLiteral("fg_civilian_road");
        n.locationTitle = QStringLiteral("1940年5月18日 · 法国公路");
        n.musicKey      = QStringLiteral("fallgelb_village");
        n.type          = NodeType::Choice;
        n.text =
            QStringLiteral("公路完全被堵死了。\n\n"
            "不是军队——是平民。成千上万的平民正在逃亡：推着婴儿车的年轻母亲、\n"
            "骑着自行车的老人、赶着羊群的农民，他们的马车、手推车、汽车堆在一起，\n"
            "形成了一条望不到头的逃亡长龙。\n\n"
            "孩子们在哭。大人们沉默地走着，眼神空洞。\n\n"
            "你们的装甲部队需要这条公路快速推进。");

        Choice c1;
        c1.text         = QStringLiteral("【等待】停下来等平民自行疏散");
        c1.nextNodeId   = QStringLiteral("fg_wait_civilians");
        c1.moraleDelta  = 5;
        c1.grantedFlags = {QStringLiteral("waited_for_civilians")};

        Choice c2;
        c2.text         = QStringLiteral("【强行】强行驱散平民，清出道路");
        c2.nextNodeId   = QStringLiteral("fg_push_civilians");
        c2.moraleDelta  = -25;

        n.choices = {c1, c2};
        addNode(n);
    }

    // ========================================================
    // 节点 18: 等待平民
    // ========================================================
    {
        StoryNode n;
        n.id            = QStringLiteral("fg_wait_civilians");
        n.locationTitle = QStringLiteral("1940年5月18日 · 法国公路旁");
        n.musicKey      = QStringLiteral("fallgelb_village");
        n.type          = NodeType::Narrative;
        n.text =
            QStringLiteral("你们在路边等了近一个小时。\n\n"
            "一个小女孩走到你面前，伸出手——她手里有半块饼干。"
            "她把它推向你，用法语说了什么。\n\n"
            "「她说，给你吃，因为你看上去很饿。」翻译说。\n\n"
            "你拒绝了。小女孩把饼干放回口袋，继续跟着大人走了。\n\n"
            "你看着她的背影，感觉喉咙里有什么东西堵住了。");
        n.nextNodeId = QStringLiteral("fg_dunkirk_race");
        addNode(n);
    }

    // ========================================================
    // 节点 19: 驱散平民
    // ========================================================
    {
        StoryNode n;
        n.id            = QStringLiteral("fg_push_civilians");
        n.locationTitle = QStringLiteral("1940年5月18日 · 法国公路");
        n.musicKey      = QStringLiteral("fallgelb_battle");
        n.type          = NodeType::Narrative;
        n.text =
            QStringLiteral("装甲车推开了路边的马车——马受了惊，一辆马车翻倒，行李散落一地。\n\n"
            "人群陷入恐慌，相互踩踏。一个老人摔倒在路边，没有人停下来扶他。\n\n"
            "你们的车队轰隆隆地从旁边驶过，尘土扬起，遮住了那老人的身影。\n\n"
            "速度是有了，但代价是什么？\n"
            "你不愿意继续想这个问题。");
        n.nextNodeId = QStringLiteral("fg_dunkirk_race");
        addNode(n);
    }

    // ========================================================
    // 节点 20: 逼近敦刻尔克
    // ========================================================
    {
        StoryNode n;
        n.id            = QStringLiteral("fg_dunkirk_race");
        n.locationTitle = QStringLiteral("1940年5月24日 · 法国北部");
        n.musicKey      = QStringLiteral("fallgelb_battle");
        n.type          = NodeType::Narrative;
        n.text =
            QStringLiteral("英法联军被你们逼到了海边。敦刻尔克——最后的出口。\n\n"
            "你们的装甲部队已经距离敦刻尔克不到二十公里。\n"
            "消灭这三十多万军队，战争就结束了，上级是这么说的。\n\n"
            "但就在这时，命令从总部传来：\n\n"
            "「停止前进！」\n\n"
            "元首的命令。所有装甲部队停止推进，等待步兵跟上。\n\n"
            "没有人知道为什么。");
        n.nextNodeId = QStringLiteral("fg_halt_order");
        addNode(n);
    }

    // ========================================================
    // 节点 21: 停止令 (选择)
    // ========================================================
    {
        StoryNode n;
        n.id            = QStringLiteral("fg_halt_order");
        n.locationTitle = QStringLiteral("1940年5月24日 · 敦刻尔克外围");
        n.musicKey      = QStringLiteral("fallgelb");
        n.type          = NodeType::Choice;
        n.text =
            QStringLiteral("你的连长把命令摔在地图上。\"这是什么命令？！我们明明可以一举歼灭他们！\"\n\n"
            "从望远镜里，你可以看到海边的英国士兵正在涌上船只。成千上万人正在撤离，\n"
            "而你们只能站在这里看着。\n\n"
            "海峡里，各种各样的船只正在来回——游艇、拖网渔船、渡轮，\n"
            "所有能开动的英国船只都来了。\n\n"
            "三天后、五天后、八天后……三十三万人就这样从你眼皮底下逃走了。");

        Choice c1;
        c1.text         = QStringLiteral("【服从】遵守命令，等待后续指示");
        c1.nextNodeId   = QStringLiteral("fg_watching_evacuation");

        Choice c2;
        c2.text         = QStringLiteral("【质疑】向上级质疑停止令的意义");
        c2.nextNodeId   = QStringLiteral("fg_question_order");
        c2.moraleDelta  = 5;

        n.choices = {c1, c2};
        addNode(n);
    }

    // ========================================================
    // 节点 22: 目睹大撤退
    // ========================================================
    {
        StoryNode n;
        n.id            = QStringLiteral("fg_watching_evacuation");
        n.locationTitle = QStringLiteral("1940年5月26日-6月4日 · 敦刻尔克海岸");
        n.musicKey      = QStringLiteral("fallgelb");
        n.type          = NodeType::Narrative;
        n.text =
            QStringLiteral("九天。\n\n"
            "你们就这样看着。海滩上的英国士兵在漫长的队伍中等待，有人中弹死去，\n"
            "有人被淹死，但更多的人上了船，离开了。\n\n"
            "德国空军来轰炸，但英国皇家空军从天而降，空战在你头顶展开。\n\n"
            "你看见一架飞机被击中，旋转着坠入海中，分不清是英国人还是德国人。\n\n"
            "三十三万三千人，从你眼前消失在了海峡那头。\n"
            "那些人将会再次出现——只是不知道在哪里，在什么时候。");
        n.nextNodeId = QStringLiteral("fg_france_falls");
        addNode(n);
    }

    // ========================================================
    // 节点 23: 质疑命令
    // ========================================================
    {
        StoryNode n;
        n.id            = QStringLiteral("fg_question_order");
        n.locationTitle = QStringLiteral("1940年5月24日 · 前线司令部");
        n.musicKey      = QStringLiteral("fallgelb");
        n.type          = NodeType::Narrative;
        n.text =
            QStringLiteral("你向连长提出了疑问。他沉默了很长时间。\n\n"
            "「有人说，」他低声说，「元首认为英国人会主动谈判。他不想把英国人逼上绝路。\n"
            "也有人说，这片地形不利于装甲部队作战。」\n\n"
            "他顿了顿：「还有人说……」他没有说完这句话。\n\n"
            "后来的历史证明，那三十三万人里有许多人会在诺曼底、在北非、在最终的柏林，"
            "与我们再次相遇。\n\n"
            "「管好你自己的事。」连长最后说。");
        n.nextNodeId = QStringLiteral("fg_watching_evacuation");
        addNode(n);
    }

    // ========================================================
    // 节点 24: 法国陷落
    // ========================================================
    {
        StoryNode n;
        n.id            = QStringLiteral("fg_france_falls");
        n.locationTitle = QStringLiteral("1940年6月22日 · 贡比涅森林");
        n.musicKey      = QStringLiteral("fallgelb_triumph");
        n.type          = NodeType::Choice;
        n.text =
            QStringLiteral("法国投降了。\n\n"
            "就在第一次世界大战签署停战协定的同一节火车车厢里，法国签署了投降书。\n"
            "元首是特意安排的这一切，这个细节让所有德国士兵都感到了某种……满足？\n\n"
            "你站在巴黎的街头。埃菲尔铁塔就在你面前。国旗已经升上了铁塔。\n\n"
            "六周，你穿越了你父亲的父亲一代人用四年都未能穿越的防线。\n\n"
            "「这就是胜利。」身旁有人说。\n\n"
            "但你环顾四周——空洞的街道，紧闭的窗户，躲在窗帘后面的目光，\n"
            "还有脑海中，那一张又一张的面孔。");

        Choice c1;
        c1.text         = QStringLiteral("【庆祝】参加胜利游行，这是应得的荣耀");
        c1.nextNodeId   = QStringLiteral("fg_victory_parade");

        Choice c2;
        c2.text         = QStringLiteral("【独处】离开人群，独自坐在废墟旁");
        c2.nextNodeId   = QStringLiteral("fg_reflection");
        c2.grantedFlags = {QStringLiteral("reflected")};

        n.choices = {c1, c2};
        addNode(n);
    }

    // ========================================================
    // 节点 25: 胜利游行
    // ========================================================
    {
        StoryNode n;
        n.id            = QStringLiteral("fg_victory_parade");
        n.locationTitle = QStringLiteral("1940年6月22日 · 巴黎");
        n.musicKey      = QStringLiteral("fallgelb_triumph");
        n.type          = NodeType::Narrative;
        n.text =
            QStringLiteral("鼓声，军乐，整齐的步伐。\n\n"
            "你走在凯旋门前，你的靴底踏在巴黎的石板路上。"
            "路旁的法国人站在那里——不是欢呼，只是沉默地看着。\n\n"
            "一个老人转过身去，背对着你们，直到你们走过。\n\n"
            "那天晚上，你独自一人坐在塞纳河边。河水倒映着夜色，"
            "远处的灯光把水面染成金色。\n\n"
            "你想给母亲写信，告诉她你还活着。\n"
            "但你不知道该从哪里开始写，也不知道该把什么告诉她，\n"
            "又该把什么永远埋在心里。");
        n.nextNodeId = QStringLiteral("fg_victory_end");
        addNode(n);
    }

    // ========================================================
    // 节点 26: 独自思考
    // ========================================================
    {
        StoryNode n;
        n.id            = QStringLiteral("fg_reflection");
        n.locationTitle = QStringLiteral("1940年6月22日 · 巴黎郊外");
        n.musicKey      = QStringLiteral("fallgelb");
        n.type          = NodeType::Narrative;
        n.text =
            QStringLiteral("你一个人走进了一条小巷。\n\n"
            "墙上有弹孔，地上还有上周战斗留下的玻璃碎片。\n"
            "一个法国小女孩从一扇破门里探出头来，看到你，立刻缩了回去，\n"
            "然后是一声闩门声。\n\n"
            "你坐在台阶上，把钢盔放在膝盖上。\n\n"
            "你数了数：从5月10日到今天，四十三天。\n"
            "四十三天里，你亲眼看到多少人死去，你已经记不清了。\n"
            "但那些面孔——弗里茨的腿、汉斯的尸体、法国少尉的眼睛、那个老妇人的猫——\n"
            "那些面孔，你有预感，会一直跟着你。\n\n"
            "这就是胜利。");
        n.nextNodeId = QStringLiteral("fg_victory_end");
        addNode(n);
    }

    // ========================================================
    // 节点 27: 胜利结局
    // ========================================================
    {
        StoryNode n;
        n.id            = QStringLiteral("fg_victory_end");
        n.locationTitle = QStringLiteral("黄色方案 — 完成");
        n.musicKey      = QStringLiteral("fallgelb_triumph");
        n.type          = NodeType::Ending;
        n.isVictory     = true;
        n.text =
            QStringLiteral("【战役结束 — 胜利】\n\n"
            "黄色方案以德国的决定性胜利告终。六周内，法国沦陷，英国远征军仓皇撤回本土。\n\n"
            "然而，敦刻尔克的三十三万人逃脱了。他们将会回来。\n\n"
            "更重要的是：这场战争远未结束。\n\n"
            "法国的战死者：约九万人。\n"
            "德国的战死者：约四万九千人。\n"
            "平民的死亡与流离：无法计数。\n\n"
            "「胜利」这个词的背后，是这片土地上每一滴流下的血，\n"
            "是每一个倒下的人曾经爱过的家、挂念的人、未竟的梦。\n\n"
            "下一章即将开启。");
        addNode(n);
    }

    // ========================================================
    // 节点 28: 失败结局
    // ========================================================
    {
        StoryNode n;
        n.id        = QStringLiteral("fg_defeat");
        n.locationTitle = QStringLiteral("黄色方案 — 失败");
        n.musicKey  = QStringLiteral("defeat_theme");
        n.type      = NodeType::Ending;
        n.isDefeat  = true;
        n.text =
            QStringLiteral("【战役结束 — 阵亡】\n\n"
            "你倒下了。\n\n"
            "不是在巴黎，不是在胜利游行的队伍里。\n"
            "是在一处不知名的法国农村，在战壕的泥土里，在某个无名的下午。\n\n"
            "你的名字会出现在某份伤亡报告上，然后消失在战争的滚滚洪流中。\n\n"
            "战争不会因为你的死而停止。");
        addNode(n);
    }
}
