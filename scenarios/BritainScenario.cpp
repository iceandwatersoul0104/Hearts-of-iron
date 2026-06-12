#include "BritainScenario.h"
#include "../core/StoryNode.h"

QString BritainScenario::startNodeId(PlayerClass cls) const {
    if (cls == PlayerClass::BomberPilot)
        return QStringLiteral("br_briefing_bomber");
    return QStringLiteral("br_briefing_fighter");
}

void BritainScenario::initialize() {

    // ========================================================
    // 节点 1: 战斗机飞行员简报
    // ========================================================
    {
        StoryNode n;
        n.id            = QStringLiteral("br_briefing_fighter");
        n.locationTitle = QStringLiteral("1940年7月10日 · 法国加莱机场");
        n.musicKey      = QStringLiteral("britain");
        n.type          = NodeType::Narrative;
        n.text =
            QStringLiteral("加莱机场的简报室里，年轻的飞行员们挤在一起，听着作战简报。\n\n"
            "你驾驶Bf-109战斗机，隶属第三航空队。窗外，你的座机正停在跑道上，\n"
            "银灰色的机翼在七月的阳光下发着光。\n\n"
            "「我们的任务，」中队长说，「是消灭英国皇家空军，夺取英吉利海峡制空权。\n"
            "没有制空权，就没有海狮计划，就没有英国的投降。」\n\n"
            "任务听起来简单。但中队里所有人都知道，英国飞行员和我们一样出色，\n"
            "而他们是在本土上空作战——被击落的英国飞行员可以跳伞回到地面，\n"
            "而我们将坠入海峡。\n\n"
            "「起飞！」");
        n.nextNodeId = QStringLiteral("br_first_scramble");
        addNode(n);
    }

    // ========================================================
    // 节点 2: 轰炸机飞行员简报
    // ========================================================
    {
        StoryNode n;
        n.id            = QStringLiteral("br_briefing_bomber");
        n.locationTitle = QStringLiteral("1940年8月13日 · 法国机场");
        n.musicKey      = QStringLiteral("britain");
        n.type          = NodeType::Narrative;
        n.text =
            QStringLiteral("你是He-111轰炸机的驾驶员，今天是「鹰日」——对英国的大规模轰炸行动正式开始。\n\n"
            "目标清单摆在你面前：英国皇家空军的雷达站、机场、指挥中心。\n"
            "这是合法的军事目标，情报官这么告诉你们。\n\n"
            "但你看着地图，看着那些密密麻麻的英国城市，心里有什么东西不安地动了一下。\n\n"
            "飞机跑道很长，你推大油门，庞大的机体缓缓加速，\n"
            "英吉利海峡就在前方，海峡对面，是英国。");
        n.nextNodeId = QStringLiteral("br_channel_crossing");
        addNode(n);
    }

    // ========================================================
    // 节点 3: 第一次紧急起飞 (战斗机)
    // ========================================================
    {
        StoryNode n;
        n.id            = QStringLiteral("br_first_scramble");
        n.locationTitle = QStringLiteral("1940年7月12日 · 英吉利海峡上空");
        n.musicKey      = QStringLiteral("britain_air");
        n.type          = NodeType::Choice;
        n.text =
            QStringLiteral("海峡上空的第一次相遇。\n\n"
            "雷达引导下，你的中队在五千米高空发现了护航英军船队的喷火式战斗机编队。\n\n"
            "阳光从背后照来，你眯起眼睛。对面有九架，你们有十二架。\n\n"
            "「红色一号，攻击！」无线电里传来命令。\n\n"
            "在你开始拉杆之前，你看见其中一架喷火式——飞行员的头盔，就在几百米外。\n"
            "那是一个和你一样的年轻人。");

        Choice c1;
        c1.text             = QStringLiteral("【主动】立即俯冲攻击，抢占优势高度");
        c1.isCombat         = true;
        c1.combatThreshold  = 50;
        c1.bonusClasses     = {PlayerClass::FighterPilot};
        c1.successNodeId    = QStringLiteral("br_first_kill");
        c1.failureNodeId    = QStringLiteral("br_first_hit");
        c1.failHpDelta      = -25;
        c1.failMoraleDelta  = -10;

        Choice c2;
        c2.text             = QStringLiteral("【谨慎】等待队形，以编队优势压制");
        c2.isCombat         = true;
        c2.combatThreshold  = 40;
        c2.bonusClasses     = {PlayerClass::FighterPilot};
        c2.successNodeId    = QStringLiteral("br_first_kill");
        c2.failureNodeId    = QStringLiteral("br_wingman_down");
        c2.failHpDelta      = -10;
        c2.failMoraleDelta  = -20;

        n.choices = {c1, c2};
        addNode(n);
    }

    // ========================================================
    // 节点 4: 穿越海峡 (轰炸机)
    // ========================================================
    {
        StoryNode n;
        n.id            = QStringLiteral("br_channel_crossing");
        n.locationTitle = QStringLiteral("1940年8月13日 · 英吉利海峡");
        n.musicKey      = QStringLiteral("britain_air");
        n.type          = NodeType::Narrative;
        n.text =
            QStringLiteral("海峡很美。从五千米高空俯瞰，那只是一条深蓝色的带子，"
            "阳光在上面波光粼粼。\n\n"
            "但你知道海面下是什么：冰冷的水、强劲的潮流。"
            "被击落在海峡上的飞行员，绝大多数都沉下去了，没有回来。\n\n"
            "「英国海岸线在前方三十公里，」领航员说，「注意防空炮火，注意战斗机。」\n\n"
            "你看见远处白色的悬崖——多佛尔白崖，像一堵墙。\n\n"
            "然后，高射炮的黑烟点在你的周围。");
        n.nextNodeId = QStringLiteral("br_target_approach");
        addNode(n);
    }

    // ========================================================
    // 节点 5: 第一次击落
    // ========================================================
    {
        StoryNode n;
        n.id            = QStringLiteral("br_first_kill");
        n.locationTitle = QStringLiteral("1940年7月12日 · 英吉利海峡");
        n.musicKey      = QStringLiteral("britain_air");
        n.type          = NodeType::Narrative;
        n.text =
            QStringLiteral("机炮的短促点射，那架喷火式冒出一道浓烟，开始侧翻。\n\n"
            "你看见飞行员跳伞了。白色的伞花在海峡上方缓缓张开。\n"
            "那人会漂到哪里，是英国岸边还是海峡中央？你不知道。\n\n"
            "「击落一架！」你的无线电里传来中队的欢呼。\n\n"
            "你没有跟着欢呼。座舱里除了发动机的轰鸣，只有你自己的呼吸声。\n\n"
            "那是你第一次击落一架飞机。你没有感觉到你以为自己会感觉到的东西——\n"
            "不是骄傲，不是恐惧，只是一种奇异的空白。");
        n.nextNodeId = QStringLiteral("br_fuel_crisis");
        addNode(n);
    }

    // ========================================================
    // 节点 6: 第一次被击中
    // ========================================================
    {
        StoryNode n;
        n.id            = QStringLiteral("br_first_hit");
        n.locationTitle = QStringLiteral("1940年7月12日 · 英吉利海峡");
        n.musicKey      = QStringLiteral("britain_air");
        n.type          = NodeType::Narrative;
        n.text =
            QStringLiteral("你没看见那架喷火式从哪里冲出来。\n\n"
            "一连串击打声——机翼中弹，冷却液开始泄漏，仪表盘上的温度迅速攀升。\n\n"
            "「红色三号，你中弹了，立即返回！」无线电里队长的声音。\n\n"
            "你拉住操纵杆，座机开始颤抖。海峡在你下方。那片水很冷，你知道。\n\n"
            "你咬着牙，把受伤的飞机开回了加莱——但只差一点。\n"
            "落地时，轮胎爆裂，飞机在跑道上打转，最终停下。\n\n"
            "你从座舱里爬出来，双腿发软，在跑道上坐了很久，什么也没有想。");
        n.nextNodeId = QStringLiteral("br_fuel_crisis");
        addNode(n);
    }

    // ========================================================
    // 节点 7: 僚机坠落
    // ========================================================
    {
        StoryNode n;
        n.id            = QStringLiteral("br_wingman_down");
        n.locationTitle = QStringLiteral("1940年7月12日 · 英吉利海峡");
        n.musicKey      = QStringLiteral("britain_air");
        n.type          = NodeType::Narrative;
        n.text =
            QStringLiteral("你看见了。\n\n"
            "库尔特的飞机中弹，发动机舱喷出火焰。他的飞机开始翻滚，\n"
            "向着海面急速下坠。\n\n"
            "他跳伞了。伞张开了。他在海峡中央，缓缓下降。\n\n"
            "你的燃油不够做搜救了。没有任何人的燃油够。\n\n"
            "你带着编队返回基地。没有人在无线电里说话。\n\n"
            "库尔特来自科隆，家里有一个六个月大的儿子，他给我们看过照片。\n"
            "海峡的水温，七月份大约是十七度。在那种水里，人可以活多久？");
        n.nextNodeId = QStringLiteral("br_fuel_crisis");
        addNode(n);
    }

    // ========================================================
    // 节点 8: 燃油危机 (选择)
    // ========================================================
    {
        StoryNode n;
        n.id            = QStringLiteral("br_fuel_crisis");
        n.locationTitle = QStringLiteral("1940年7月25日 · 加莱基地");
        n.musicKey      = QStringLiteral("britain");
        n.type          = NodeType::Choice;
        n.text =
            QStringLiteral("Bf-109的续航是一个致命的问题。\n\n"
            "从加莱到伦敦再回来，飞机只能在目标区域上空停留十到二十分钟，\n"
            "然后燃油就会进入危险区间。\n\n"
            "今天的任务：护送轰炸机群深入英国内陆，攻击皇家空军机场。\n\n"
            "中队长说：「如果在英国上空燃油告急，你有两个选择——」\n"
            "他没有把话说完，因为大家都知道：\n"
            "返回，或者在英国领土上跳伞，成为俘虏。");

        Choice c1;
        c1.text             = QStringLiteral("【坚持】尽量延长在英国上空的作战时间");
        c1.isCombat         = true;
        c1.combatThreshold  = 55;
        c1.bonusClasses     = {PlayerClass::FighterPilot};
        c1.successNodeId    = QStringLiteral("br_escort_success");
        c1.failureNodeId    = QStringLiteral("br_fuel_emergency");
        c1.failHpDelta      = -20;
        c1.failMoraleDelta  = -15;

        Choice c2;
        c2.text             = QStringLiteral("【保守】严格控制燃油，提前折返");
        c2.nextNodeId       = QStringLiteral("br_escort_partial");
        c2.moraleDelta      = -10;

        n.choices = {c1, c2};
        addNode(n);
    }

    // ========================================================
    // 节点 9: 轰炸目标区域 (轰炸机路线)
    // ========================================================
    {
        StoryNode n;
        n.id            = QStringLiteral("br_target_approach");
        n.locationTitle = QStringLiteral("1940年8月13日 · 英国上空");
        n.musicKey      = QStringLiteral("britain_air");
        n.type          = NodeType::Choice;
        n.text =
            QStringLiteral("目标是多佛附近的雷达站。\n\n"
            "但在高射炮的烟雾中，领航员告诉你：「目标发现偏差，实际目标在那个村庄东侧三公里。」\n\n"
            "你向右下方看去——那是一个英国小镇，屋顶的颜色在阳光下橙红一片，\n"
            "像拼图一样美丽。\n\n"
            "「确认目标坐标？」你问。\n"
            "「确认，」他说，「雷达站就在那里。」\n\n"
            "但你的眼睛告诉你，那里的房子太密集了，雷达站和民居之间的距离……");

        Choice c1;
        c1.text             = QStringLiteral("【执行】按坐标投弹，执行命令");
        c1.nextNodeId       = QStringLiteral("br_bombs_dropped");
        c1.moraleDelta      = -25;

        Choice c2;
        c2.text             = QStringLiteral("【质疑】拒绝投弹，要求重新确认目标");
        c2.nextNodeId       = QStringLiteral("br_target_questioned");
        c2.grantedFlags     = {QStringLiteral("refused_bomb")};

        n.choices = {c1, c2};
        addNode(n);
    }

    // ========================================================
    // 节点 10: 轰炸执行
    // ========================================================
    {
        StoryNode n;
        n.id            = QStringLiteral("br_bombs_dropped");
        n.locationTitle = QStringLiteral("1940年8月13日 · 英国多佛附近");
        n.musicKey      = QStringLiteral("britain_air");
        n.type          = NodeType::Narrative;
        n.text =
            QStringLiteral("「投弹！」\n\n"
            "飞机因为突然减轻重量而轻微上跳。在你下方，几百公斤的炸弹正在向地面坠落。\n\n"
            "爆炸的火光从高空看起来很小，像一朵橙色的花，然后是烟柱。\n\n"
            "那橙色花开放的地方，有建筑物，有街道，有此前一秒钟还在走路的人。\n\n"
            "飞机掉头，向法国飞去。领航员说「目标摧毁」，声音很平静。\n\n"
            "你没有说任何话。你不知道可以说什么。\n\n"
            "后来的报告说，那是一个集市，不是雷达站。");
        n.nextNodeId = QStringLiteral("br_london_order");
        addNode(n);
    }

    // ========================================================
    // 节点 11: 拒绝投弹
    // ========================================================
    {
        StoryNode n;
        n.id            = QStringLiteral("br_target_questioned");
        n.locationTitle = QStringLiteral("1940年8月13日 · 英国上空");
        n.musicKey      = QStringLiteral("britain_air");
        n.type          = NodeType::Narrative;
        n.text =
            QStringLiteral("「这里有平民聚居区！重新确认目标坐标！」\n\n"
            "无线电里沉默了几秒，然后是上级的怒吼：\n"
            "「按命令执行！」\n\n"
            "你在错误的坐标上空盘旋了两圈，没有投弹，然后带着炸弹飞回去了。\n\n"
            "着陆后，你被叫进了指挥官的办公室。那是一次很难听的谈话。\n\n"
            "但你睡得着觉。");
        n.nextNodeId = QStringLiteral("br_london_order");
        addNode(n);
    }

    // ========================================================
    // 节点 12: 护送成功
    // ========================================================
    {
        StoryNode n;
        n.id            = QStringLiteral("br_escort_success");
        n.locationTitle = QStringLiteral("1940年8月 · 英国上空");
        n.musicKey      = QStringLiteral("britain_air");
        n.type          = NodeType::Narrative;
        n.text =
            QStringLiteral("你成功护送轰炸机群完成了攻击，在燃油警报响起之前，一路驾机返回法国。\n\n"
            "但代价是沉重的：中队损失了两架飞机。卡尔和迪特尔在返回途中被喷火式截击，"
            "都没有时间跳伞。\n\n"
            "晚上，空出来的两张行军床就在营房角落里。\n"
            "没有人去碰那两张床上还在的私人物品。\n\n"
            "晚饭时，食堂里比平时安静很多。");
        n.nextNodeId = QStringLiteral("br_london_order");
        addNode(n);
    }

    // ========================================================
    // 节点 13: 燃油紧急
    // ========================================================
    {
        StoryNode n;
        n.id            = QStringLiteral("br_fuel_emergency");
        n.locationTitle = QStringLiteral("1940年8月 · 英国上空");
        n.musicKey      = QStringLiteral("britain_air");
        n.type          = NodeType::Narrative;
        n.text =
            QStringLiteral("燃油警报灯亮起，你距离法国还有三十公里。\n\n"
            "你拼命降低油门，保持最经济的飞行姿态。发动机的声音听起来很健康，\n"
            "但油表指针还在不断下滑。\n\n"
            "海峡出现在你下方。你以滑翔的姿势越过海面，\n"
            "发动机在距离海岸还有两公里时熄火。\n\n"
            "你以死飞姿态迫降在一片法国的农田里，翻了半个跟头，停在一条水沟边。\n\n"
            "你从倒扣的座舱里爬出来，在田野里躺了很久，看着天上的云。\n"
            "活着，真好。");
        n.nextNodeId = QStringLiteral("br_london_order");
        addNode(n);
    }

    // ========================================================
    // 节点 14: 护送折返
    // ========================================================
    {
        StoryNode n;
        n.id            = QStringLiteral("br_escort_partial");
        n.locationTitle = QStringLiteral("1940年8月 · 英国上空");
        n.musicKey      = QStringLiteral("britain_air");
        n.type          = NodeType::Narrative;
        n.text =
            QStringLiteral("你提前折返了，轰炸机群继续深入英国，没有护航。\n\n"
            "回到基地，你站在跑道上，听着远处高射炮的声音，等待。\n\n"
            "轰炸机群回来时少了三架。幸存的飞行员报告说：'喷火式在没有护航之后就扑上来了。'\n\n"
            "没有人直接责怪你。但你知道。");
        n.nextNodeId = QStringLiteral("br_london_order");
        addNode(n);
    }

    // ========================================================
    // 节点 15: 伦敦轰炸令 (选择)
    // ========================================================
    {
        StoryNode n;
        n.id            = QStringLiteral("br_london_order");
        n.locationTitle = QStringLiteral("1940年9月7日 · 基地简报室");
        n.musicKey      = QStringLiteral("britain");
        n.type          = NodeType::Choice;
        n.text =
            QStringLiteral("1940年9月7日，新命令下达——目标改变。\n\n"
            "不再是皇家空军的机场和雷达站，而是伦敦本身。\n\n"
            "中队长展开地图，手指点在伦敦的码头区：\n"
            "「集中轰炸伦敦，迫使英国人屈服或让皇家空军全力出来应战。」\n\n"
            "简报室里有人看了看地图上伦敦的大小——那是一个住着八百多万人的城市。\n\n"
            "「有问题吗？」中队长扫视众人。");

        Choice c1;
        c1.text         = QStringLiteral("【服从】没有问题，执行命令");
        c1.nextNodeId   = QStringLiteral("br_london_blitz");
        c1.moraleDelta  = -20;

        Choice c2;
        c2.text         = QStringLiteral("【私下质疑】私下向中队长表达对轰炸平民的顾虑");
        c2.nextNodeId   = QStringLiteral("br_quiet_protest");
        c2.grantedFlags = {QStringLiteral("questioned_london")};

        n.choices = {c1, c2};
        addNode(n);
    }

    // ========================================================
    // 节点 16: 伦敦大轰炸
    // ========================================================
    {
        StoryNode n;
        n.id            = QStringLiteral("br_london_blitz");
        n.locationTitle = QStringLiteral("1940年9月7日夜 · 伦敦上空");
        n.musicKey      = QStringLiteral("britain_night");
        n.type          = NodeType::Narrative;
        n.text =
            QStringLiteral("夜间。伦敦在你下方燃烧。\n\n"
            "泰晤士河的水面反射着火光，码头区已经是一片火海。\n"
            "从五千米高空看下去，那像是某种壮观的景象，\n"
            "但你知道那火光里有什么。\n\n"
            "「投弹，」领航员的声音平静得像在背课文，「投弹完毕，返航。」\n\n"
            "飞机划过夜空，向法国飞去。\n\n"
            "第二天早上，报纸上说，昨晚有四百三十六名伦敦平民死亡，\n"
            "一千六百余人受伤，其中包括大量儿童。\n\n"
            "报纸没有说这些人的名字。");
        n.nextNodeId = QStringLiteral("br_raf_counterattack");
        addNode(n);
    }

    // ========================================================
    // 节点 17: 私下抗议
    // ========================================================
    {
        StoryNode n;
        n.id            = QStringLiteral("br_quiet_protest");
        n.locationTitle = QStringLiteral("1940年9月6日 · 中队长办公室");
        n.musicKey      = QStringLiteral("britain");
        n.type          = NodeType::Narrative;
        n.text =
            QStringLiteral("中队长听完你的话，沉默了一会儿。\n\n"
            "「你认为这场战争是在打仗，」他说，「你错了。这场战争是在逼英国人屈服。\n"
            "打烂他们的城市，打断他们的意志，这才是目的。」\n\n"
            "他顿了顿：「你也知道，他们也在轰炸我们的城市。汉堡，不莱梅，\n"
            "这是战争，不是比赛。」\n\n"
            "「但那些是平民。」你说。\n\n"
            "「我知道，」中队长说，眼神里有什么东西，你没能读懂。\n"
            "「我也知道。但你还是要飞。」\n\n"
            "你飞了。");
        n.nextNodeId = QStringLiteral("br_london_blitz");
        addNode(n);
    }

    // ========================================================
    // 节点 18: 皇家空军反击 (战斗)
    // ========================================================
    {
        StoryNode n;
        n.id            = QStringLiteral("br_raf_counterattack");
        n.locationTitle = QStringLiteral("1940年9月15日 · 英吉利海峡上空");
        n.musicKey      = QStringLiteral("britain_air");
        n.type          = NodeType::Choice;
        n.text =
            QStringLiteral("1940年9月15日，后来被英国人称为「不列颠之战日」。\n\n"
            "皇家空军发动了他们最大规模的反击。你的中队刚过海峡，就遭遇了庞大的拦截编队——\n"
            "不是十架，不是二十架，是超过两百架的喷火式和飓风式。\n\n"
            "无线电里乱作一团。炸弹编队被截击，护航战斗机在混战中四散。\n\n"
            "你的座机被两架喷火式夹击，油门全开也无法摆脱。");

        Choice c1;
        c1.text             = QStringLiteral("【战斗】硬扛，用技术摆脱追击并还击");
        c1.isCombat         = true;
        c1.combatThreshold  = 65;
        c1.bonusClasses     = {PlayerClass::FighterPilot};
        c1.successNodeId    = QStringLiteral("br_survived_day");
        c1.failureNodeId    = QStringLiteral("br_shot_down");
        c1.failHpDelta      = -40;
        c1.failMoraleDelta  = -20;

        Choice c2;
        c2.text             = QStringLiteral("【撤退】放弃编队，全速逃回法国");
        c2.nextNodeId       = QStringLiteral("br_retreat");
        c2.moraleDelta      = -20;
        c2.hpDelta          = -15;

        n.choices = {c1, c2};
        addNode(n);
    }

    // ========================================================
    // 节点 19: 撤退
    // ========================================================
    {
        StoryNode n;
        n.id            = QStringLiteral("br_retreat");
        n.locationTitle = QStringLiteral("1940年9月15日 · 海峡上空");
        n.musicKey      = QStringLiteral("britain_air");
        n.type          = NodeType::Narrative;
        n.text =
            QStringLiteral("你丢下编队，向法国全速飞去。\n\n"
            "一架喷火式在后面追了十公里，然后放弃了——它也快到燃油极限了。\n\n"
            "你回到基地，汇报称发动机故障被迫返航。\n\n"
            "那一天，皇家空军击落了六十一架德国飞机，德国声称击落了一百八十架英国飞机——\n"
            "实际数字是二十六架。\n\n"
            "你知道这场制空权之争的天平正在倾斜，但你没有说出来。");
        n.nextNodeId = QStringLiteral("br_attrition");
        addNode(n);
    }

    // ========================================================
    // 节点 20: 活下来的那天
    // ========================================================
    {
        StoryNode n;
        n.id            = QStringLiteral("br_survived_day");
        n.locationTitle = QStringLiteral("1940年9月15日 · 加莱基地");
        n.musicKey      = QStringLiteral("britain");
        n.type          = NodeType::Narrative;
        n.text =
            QStringLiteral("你摆脱了追击，击落了一架喷火式，\n"
            "然后在燃油告急时强行返回了基地。\n\n"
            "落地后，地勤人员清点机体弹孔：七个，还好没打穿油箱。\n\n"
            "那天，中队长点名：十二个人，回来了八个。\n"
            "四个名字被念到，没有人回应。\n\n"
            "中队长收起了那张名单。大家去食堂吃晚饭。\n"
            "有人喝酒，有人沉默，有人哼起歌来，马上被人制止了。");
        n.nextNodeId = QStringLiteral("br_attrition");
        addNode(n);
    }

    // ========================================================
    // 节点 21: 被击落
    // ========================================================
    {
        StoryNode n;
        n.id            = QStringLiteral("br_shot_down");
        n.locationTitle = QStringLiteral("1940年9月15日 · 坠落");
        n.musicKey      = QStringLiteral("britain_air");
        n.type          = NodeType::Narrative;
        n.text =
            QStringLiteral("发动机中弹，舱内开始冒烟。\n\n"
            "你用尽力气推开座舱盖，跳了出去。寒风把你猛地一拉，\n"
            "降落伞张开，白色的伞顶在蓝天下飘荡。\n\n"
            "你在英国的土地上落下，膝盖摔破了，手在颤抖。\n\n"
            "一名英国农场主拿着猎枪向你走来，后面跟着他的两个孩子。\n"
            "「放下武器。」他用英语说。\n\n"
            "这是你第一次以战俘身份活着。");
        n.nextNodeId = QStringLiteral("br_pow_return");
        addNode(n);
    }

    // ========================================================
    // 节点 22: 战俘归来
    // ========================================================
    {
        StoryNode n;
        n.id            = QStringLiteral("br_pow_return");
        n.locationTitle = QStringLiteral("1940年10月 · 英国战俘营");
        n.musicKey      = QStringLiteral("britain");
        n.type          = NodeType::Narrative;
        n.text =
            QStringLiteral("战俘营的生活比你预想的好一些。英国人遵守日内瓦公约，这让你有点意外。\n\n"
            "营地里有来自各个中队的德国飞行员，一个来自汉堡的人认识你。\n"
            "你们谈起伦敦——他的轰炸机曾飞过伦敦，他告诉你他从那以后再没睡过一整晚觉。\n\n"
            "三个月后，通过换俘，你回到了法国。\n\n"
            "返回基地那天，没有人迎接，因为大家都忙着准备下一次出击。\n"
            "战争不等人。");
        n.nextNodeId = QStringLiteral("br_attrition");
        addNode(n);
    }

    // ========================================================
    // 节点 23: 消耗战
    // ========================================================
    {
        StoryNode n;
        n.id            = QStringLiteral("br_attrition");
        n.locationTitle = QStringLiteral("1940年10月 · 加莱基地");
        n.musicKey      = QStringLiteral("britain");
        n.type          = NodeType::Narrative;
        n.text =
            QStringLiteral("十月了。法国的秋天来了，海峡上开始出现大雾。\n\n"
            "战况发生了变化。德国空军在这场消耗战中损失了一千八百多架飞机，\n"
            "训练有素的飞行员越来越难以补充。\n\n"
            "而皇家空军顽强地活着。他们的飞机比我们少，损失比我们少，\n"
            "因为他们每次都在本土上空作战。\n\n"
            "从你到这里以来，中队里的人换了三批——\n"
            "只有你，和中队长，以及地勤兵里的老克劳斯，还在。\n\n"
            "你已经不去记新人的名字了。");
        n.nextNodeId = QStringLiteral("br_final_mission");
        addNode(n);
    }

    // ========================================================
    // 节点 24: 最后的任务 (选择)
    // ========================================================
    {
        StoryNode n;
        n.id            = QStringLiteral("br_final_mission");
        n.locationTitle = QStringLiteral("1940年10月31日 · 最后一次出击");
        n.musicKey      = QStringLiteral("britain_air");
        n.type          = NodeType::Choice;
        n.text =
            QStringLiteral("10月31日。海狮计划已经被无限期推迟——制空权未能夺取，登陆英国的设想只是一个设想。\n\n"
            "但今天还有任务：最后一次大规模护航轰炸。\n\n"
            "你坐在座舱里，看着地图上英国的轮廓。四个月来，\n"
            "你飞过这片海峡无数次，每次都看着战友一个个离开，\n"
            "却依然飞回来。\n\n"
            "也许这次也会回来。");

        Choice c1;
        c1.text             = QStringLiteral("【出击】驾机升空，执行最后任务");
        c1.isCombat         = true;
        c1.combatThreshold  = 50;
        c1.bonusClasses     = {PlayerClass::FighterPilot, PlayerClass::BomberPilot};
        c1.successNodeId    = QStringLiteral("br_victory_end");
        c1.failureNodeId    = QStringLiteral("br_final_crash");
        c1.failHpDelta      = -50;
        c1.failMoraleDelta  = -30;

        Choice c2;
        c2.text             = QStringLiteral("【反思】在地图前停留，思考这一切的意义");
        c2.nextNodeId       = QStringLiteral("br_reflection_end");

        n.choices = {c1, c2};
        addNode(n);
    }

    // ========================================================
    // 节点 25: 胜利结局
    // ========================================================
    {
        StoryNode n;
        n.id            = QStringLiteral("br_victory_end");
        n.locationTitle = QStringLiteral("不列颠空战 — 完成");
        n.musicKey      = QStringLiteral("britain_triumph");
        n.type          = NodeType::Ending;
        n.isVictory     = true;
        n.text =
            QStringLiteral("【战役结束 — 生存】\n\n"
            "你活过了不列颠空战。\n\n"
            "但夺取制空权的目标没有实现。皇家空军依然存在，英国依然存在。\n"
            "海狮计划，那个入侵英国本土的计划，被永久搁置了。\n\n"
            "这场空战中，德国损失飞行员约二千七百人，\n"
            "英国皇家空军损失约一千五百名飞行员。\n"
            "伦敦等城市的轰炸造成约四万三千名英国平民死亡。\n\n"
            "你们赢得了所有的空战，却输掉了整场战役。\n\n"
            "而那些被烧毁的伦敦街道，那些死去的英国平民，\n"
            "成为了英国人永不退缩的理由。\n\n"
            "战争制造了英雄，也制造了地狱。你见过的是哪一种？\n\n"
            "下一章即将开启。");
        addNode(n);
    }

    // ========================================================
    // 节点 26: 反思结局
    // ========================================================
    {
        StoryNode n;
        n.id            = QStringLiteral("br_reflection_end");
        n.locationTitle = QStringLiteral("不列颠空战 — 完成");
        n.musicKey      = QStringLiteral("britain");
        n.type          = NodeType::Ending;
        n.isVictory     = true;
        n.text =
            QStringLiteral("【战役结束 — 生存与反思】\n\n"
            "你没有再起飞。\n\n"
            "在简报室里，你盯着地图上英国的轮廓看了很久。\n"
            "从北到南，从东到西，那片土地上有多少人因为这场战争而死，\n"
            "又有多少人将在未来死去？\n\n"
            "也许战争真的无法被任何一张地图、任何一次胜利来合理化。\n\n"
            "战争不是棋局，死亡不是棋子。\n\n"
            "下一章即将开启。");
        addNode(n);
    }

    // ========================================================
    // 节点 27: 最终坠机 (失败)
    // ========================================================
    {
        StoryNode n;
        n.id            = QStringLiteral("br_final_crash");
        n.locationTitle = QStringLiteral("不列颠空战 — 失败");
        n.musicKey      = QStringLiteral("defeat_theme");
        n.type          = NodeType::Ending;
        n.isDefeat      = true;
        n.text =
            QStringLiteral("【战役结束 — 阵亡】\n\n"
            "你的飞机在英吉利海峡上空中弹，发动机熄火。\n\n"
            "你来不及跳伞。海峡的水是冷的，非常冷，\n"
            "像一道门，关上了。\n\n"
            "你的名字被写在那张越来越长的名单上，\n"
            "然后随着下一次点名，被缓缓念出，然后消散在海风中。");
        addNode(n);
    }
}
