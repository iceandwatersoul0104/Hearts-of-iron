#include "WolfPackScenario.h"
#include "../core/StoryNode.h"

QString WolfPackScenario::startNodeId(PlayerClass cls) const {
    if (cls == PlayerClass::BattleshipCrew)
        return QStringLiteral("wp_briefing_ship");
    return QStringLiteral("wp_briefing_sub");
}

void WolfPackScenario::initialize() {

    // ========================================================
    // 节点 1: 潜艇驾驶员简报
    // ========================================================
    {
        StoryNode n;
        n.id            = QStringLiteral("wp_briefing_sub");
        n.locationTitle = QStringLiteral("1941年 · 基尔港，德国海军基地");
        n.musicKey      = QStringLiteral("wolfpack");
        n.type          = NodeType::Narrative;
        n.text =
            QStringLiteral("U-96号潜艇静静地停在基尔港的深灰色海水里，舰体漆黑，修长，"
            "像一条沉睡的铁鱼。\n\n"
            "你是艇长，三十二岁，来自汉堡，父亲是一名渔夫。\n\n"
            "「大西洋的运输线，」邓尼茨将军的声音从简报室的广播里传来，"
            "「是英国的生命线。切断它，英国就会屈服。」\n\n"
            "任务：深入北大西洋，追踪并摧毁英美的商船护航队。\n\n"
            "你的艇员平均年龄二十一岁。他们都没有战死过——你希望他们不必学会。\n\n"
            "下潜命令下达。舱门关闭的声音像一声叹息。");
        n.nextNodeId = QStringLiteral("wp_atlantic_transit");
        addNode(n);
    }

    // ========================================================
    // 节点 2: 战列舰简报
    // ========================================================
    {
        StoryNode n;
        n.id            = QStringLiteral("wp_briefing_ship");
        n.locationTitle = QStringLiteral("1941年5月 · 威廉港，俾斯麦号");
        n.musicKey      = QStringLiteral("wolfpack");
        n.type          = NodeType::Narrative;
        n.text =
            QStringLiteral("你站在俾斯麦号的甲板上，这艘四万五千吨的战列舰是帝国最骄傲的武器。\n\n"
            "「莱茵演习行动，」舰长在晨会上宣布，「我们将突破英国的海上封锁，\n"
            "进入北大西洋，打击英国商船运输线。」\n\n"
            "俾斯麦号，全世界最强大的战列舰。艇员两千二百人，\n"
            "配备全世界最重的舰炮。\n\n"
            "你看着这艘巨舰，感到一种奇异的骄傲，也感到一种说不清的不安。\n\n"
            "「是否有问题？」\n「没有。」");
        n.nextNodeId = QStringLiteral("wp_north_sea");
        addNode(n);
    }

    // ========================================================
    // 节点 3: 大西洋过渡
    // ========================================================
    {
        StoryNode n;
        n.id            = QStringLiteral("wp_atlantic_transit");
        n.locationTitle = QStringLiteral("1941年 · 北大西洋，潜航中");
        n.musicKey      = QStringLiteral("wolfpack_deep");
        n.type          = NodeType::Narrative;
        n.text =
            QStringLiteral("潜航第三天。\n\n"
            "铁制的舱壁不断发出细微的吱嘎声，那是海水压力的声音。\n"
            "你所在的深度，每增加十米，压力就增加一个大气压。\n"
            "当你们在水下两百米时，外面的海水想要把这个铁桶压扁。\n\n"
            "艇内温度接近零度，每个人都穿着潮湿的羊毛衣，\n"
            "空气里弥漫着机油、汗水、霉菌的混合气味，\n"
            "你们把这种气味叫做「U艇香水」。\n\n"
            "「艇长，声纳接触。」");
        n.nextNodeId = QStringLiteral("wp_convoy_spotted");
        addNode(n);
    }

    // ========================================================
    // 节点 4: 北海 (战列舰路线)
    // ========================================================
    {
        StoryNode n;
        n.id            = QStringLiteral("wp_north_sea");
        n.locationTitle = QStringLiteral("1941年5月 · 丹麦海峡");
        n.musicKey      = QStringLiteral("wolfpack");
        n.type          = NodeType::Narrative;
        n.text =
            QStringLiteral("丹麦海峡，俾斯麦号和亲王欧根号编队前进。\n\n"
            "海面上漂浮着冰块，天空是铅灰色的。远处的冰山沉默地漂移。\n\n"
            "然后，雷达信号。英国皇家海军的胡德号战列巡洋舰和威尔士亲王号出现了。\n\n"
            "「右舷开炮！」\n\n"
            "第一轮齐射。海面上的冰块在炮声中震碎。\n\n"
            "然后，胡德号中弹，那艘巨舰——在几秒钟内——消失了。\n"
            "爆炸，分裂，沉没。一千四百一十五名英国水兵，只有三人生还。\n\n"
            "俾斯麦号的甲板上，沉默了很久。");
        n.nextNodeId = QStringLiteral("wp_bismarck_hunted");
        addNode(n);
    }

    // ========================================================
    // 节点 5: 发现护航队 (潜艇)
    // ========================================================
    {
        StoryNode n;
        n.id            = QStringLiteral("wp_convoy_spotted");
        n.locationTitle = QStringLiteral("1941年 · 北大西洋");
        n.musicKey      = QStringLiteral("wolfpack_deep");
        n.type          = NodeType::Choice;
        n.text =
            QStringLiteral("声纳员把耳机摘下来，脸色发白：「四十八个接触点，艇长。\n"
            "护航商船队——大型的，至少三十艘商船，八艘护卫舰。」\n\n"
            "你爬上潜望镜，在暗夜的海面上看见了船只的轮廓——一长串黑影，\n"
            "满载着食物、弹药、物资，正从美国驶向英国。\n\n"
            "「通知狼群，」你说，「我们已经找到猎物。」\n\n"
            "但猎物有血，有肉，有家人在等待。");

        Choice c1;
        c1.text             = QStringLiteral("【进攻】立即发起鱼雷攻击，击沉目标");
        c1.isCombat         = true;
        c1.combatThreshold  = 50;
        c1.bonusClasses     = {PlayerClass::SubmarineCrew};
        c1.successNodeId    = QStringLiteral("wp_torpedo_hit");
        c1.failureNodeId    = QStringLiteral("wp_torpedo_miss");
        c1.failHpDelta      = -15;
        c1.failMoraleDelta  = -10;

        Choice c2;
        c2.text             = QStringLiteral("【等待】等夜间最暗处，以最佳角度攻击");
        c2.isCombat         = true;
        c2.combatThreshold  = 40;
        c2.bonusClasses     = {PlayerClass::SubmarineCrew};
        c2.successNodeId    = QStringLiteral("wp_torpedo_hit");
        c2.failureNodeId    = QStringLiteral("wp_detected");
        c2.failHpDelta      = -25;
        c2.failMoraleDelta  = -15;

        n.choices = {c1, c2};
        addNode(n);
    }

    // ========================================================
    // 节点 6: 俾斯麦被追猎
    // ========================================================
    {
        StoryNode n;
        n.id            = QStringLiteral("wp_bismarck_hunted");
        n.locationTitle = QStringLiteral("1941年5月24日-27日 · 北大西洋");
        n.musicKey      = QStringLiteral("wolfpack");
        n.type          = NodeType::Choice;
        n.text =
            QStringLiteral("胡德号的爆炸惊动了整个英国皇家海军。\n\n"
            "从这一刻起，所有可以调动的英国军舰都在寻找俾斯麦号。\n"
            "八艘战列舰，二艘航母，数十艘巡洋舰和驱逐舰，\n"
            "在大西洋上形成了一张巨网。\n\n"
            "俾斯麦号中弹，燃油开始泄漏，一条黑色的尾迹在大西洋上画出了你们的位置。\n\n"
            "「我们无法到达法国，」舰长的声音很平静，「但我们会战斗。」");

        Choice c1;
        c1.text             = QStringLiteral("【战斗到底】坚持战斗，死而后已");
        c1.isCombat         = true;
        c1.combatThreshold  = 60;
        c1.bonusClasses     = {PlayerClass::BattleshipCrew};
        c1.successNodeId    = QStringLiteral("wp_bismarck_last_stand");
        c1.failureNodeId    = QStringLiteral("wp_bismarck_sinking");
        c1.failHpDelta      = -35;
        c1.failMoraleDelta  = -20;

        Choice c2;
        c2.text             = QStringLiteral("【突围】全速向法国方向突围");
        c2.nextNodeId       = QStringLiteral("wp_bismarck_last_stand");
        c2.hpDelta          = -20;

        n.choices = {c1, c2};
        addNode(n);
    }

    // ========================================================
    // 节点 7: 鱼雷命中
    // ========================================================
    {
        StoryNode n;
        n.id            = QStringLiteral("wp_torpedo_hit");
        n.locationTitle = QStringLiteral("1941年 · 北大西洋夜间");
        n.musicKey      = QStringLiteral("wolfpack_deep");
        n.type          = NodeType::Narrative;
        n.text =
            QStringLiteral("「鱼雷一发射！鱼雷二发射！」\n\n"
            "你在黑暗中数秒。二十秒，三十秒，四十……\n\n"
            "爆炸的声音透过舱壁传来，低沉而巨大，像是大地在呻吟。\n\n"
            "「命中！」声纳员喊道。\n\n"
            "你把眼睛贴上潜望镜。一艘万吨货轮正在缓缓下沉，"
            "它的上层甲板已经燃起大火，橙黄色的火光把夜间的海面照得通亮。\n\n"
            "在潜望镜的视野里，你看见了船上的人——"
            "有人在往海里跳，有人站在甲板边抓着护栏，有人已经在水里挣扎。\n\n"
            "那是商船的水手，不是军人。");
        n.nextNodeId = QStringLiteral("wp_survivors");
        addNode(n);
    }

    // ========================================================
    // 节点 8: 鱼雷未命中/被发现
    // ========================================================
    {
        StoryNode n;
        n.id            = QStringLiteral("wp_torpedo_miss");
        n.locationTitle = QStringLiteral("1941年 · 北大西洋");
        n.musicKey      = QStringLiteral("wolfpack_deep");
        n.type          = NodeType::Narrative;
        n.text =
            QStringLiteral("鱼雷偏离了目标，从商船船头几十米处穿过，什么也没有打到。\n\n"
            "「护卫舰发现我们了！深弹！」\n\n"
            "「全速下潜！」\n\n"
            "深水炸弹在舱外爆炸，整艘潜艇剧烈颤抖，舱壁上的铆钉噼啪作响，\n"
            "一名艇员被甩到舱壁上，嘴里流着血。\n\n"
            "「下潜到两百米，保持静默。」");
        n.nextNodeId = QStringLiteral("wp_depth_charge");
        addNode(n);
    }

    // ========================================================
    // 节点 9: 被发现
    // ========================================================
    {
        StoryNode n;
        n.id            = QStringLiteral("wp_detected");
        n.locationTitle = QStringLiteral("1941年 · 北大西洋");
        n.musicKey      = QStringLiteral("wolfpack_deep");
        n.type          = NodeType::Narrative;
        n.text =
            QStringLiteral("护卫舰的声纳找到了你。\n\n"
            "「主动声纳接触！距离一千米，接近中！」\n\n"
            "全艇进入战备状态。艇员们彼此对视，没有人说话。\n"
            "每个人都知道，接下来几个小时会发生什么。");
        n.nextNodeId = QStringLiteral("wp_depth_charge");
        addNode(n);
    }

    // ========================================================
    // 节点 10: 幸存者 (选择)
    // ========================================================
    {
        StoryNode n;
        n.id            = QStringLiteral("wp_survivors");
        n.locationTitle = QStringLiteral("1941年 · 北大西洋，击沉现场");
        n.musicKey      = QStringLiteral("wolfpack_deep");
        n.type          = NodeType::Choice;
        n.text =
            QStringLiteral("海面上有人在呼救。\n\n"
            "你从潜望镜里看见，十几个身穿救生衣的人正在冰冷的大西洋中挣扎。\n"
            "水温大约是五到八摄氏度——在这种温度下，人最多能坚持三十分钟。\n\n"
            "「艇长，」大副站在你身后，「邓尼茨的命令是不救助商船幸存者。\n"
            "护卫舰可能随时赶来。」\n\n"
            "你把眼睛从潜望镜上移开，看着艇员们的脸。\n"
            "他们都在看着你。");

        Choice c1;
        c1.text         = QStringLiteral("【人道】违反命令，浮出水面救助幸存者");
        c1.nextNodeId   = QStringLiteral("wp_save_survivors");
        c1.moraleDelta  = 15;
        c1.hpDelta      = -15;
        c1.grantedFlags = {QStringLiteral("saved_survivors")};

        Choice c2;
        c2.text         = QStringLiteral("【命令】遵守命令，立即下潜，继续任务");
        c2.nextNodeId   = QStringLiteral("wp_leave_survivors");
        c2.moraleDelta  = -30;

        n.choices = {c1, c2};
        addNode(n);
    }

    // ========================================================
    // 节点 11: 救助幸存者
    // ========================================================
    {
        StoryNode n;
        n.id            = QStringLiteral("wp_save_survivors");
        n.locationTitle = QStringLiteral("1941年 · 北大西洋，海面");
        n.musicKey      = QStringLiteral("wolfpack");
        n.type          = NodeType::Narrative;
        n.text =
            QStringLiteral("你下令浮出水面。舱门打开，海风和盐味扑面而来。\n\n"
            "你们从冰冷的海水里拉上来九个人，大多数都已经失去意识。\n"
            "其中一个用力抓住你的手，他的英语口音很重，他不停地说一句话，\n"
            "大副翻译道：「他在说谢谢你，谢谢你没有离开。」\n\n"
            "你们把他们留在一只橡皮艇里，给了他们食物和水，\n"
            "然后重新下潜。这违反了命令。\n\n"
            "你会受到军事处分，你知道。\n"
            "但那九个人还活着，你也知道。");
        n.nextNodeId = QStringLiteral("wp_depth_charge");
        addNode(n);
    }

    // ========================================================
    // 节点 12: 离开幸存者
    // ========================================================
    {
        StoryNode n;
        n.id            = QStringLiteral("wp_leave_survivors");
        n.locationTitle = QStringLiteral("1941年 · 北大西洋");
        n.musicKey      = QStringLiteral("wolfpack_deep");
        n.type          = NodeType::Narrative;
        n.text =
            QStringLiteral("「下潜，保持静默。」\n\n"
            "随着潜望镜沉入水中，你最后看到的，是那些在水里挣扎的人影，\n"
            "越来越小，越来越远，然后消失在绿色的海水中。\n\n"
            "潜艇里没有人说话。\n\n"
            "深水炸弹在夜里爆炸的声音，和那些人的呼喊声一起，\n"
            "在你脑子里持续了很久。");
        n.nextNodeId = QStringLiteral("wp_depth_charge");
        addNode(n);
    }

    // ========================================================
    // 节点 13: 俾斯麦最后的战斗
    // ========================================================
    {
        StoryNode n;
        n.id            = QStringLiteral("wp_bismarck_last_stand");
        n.locationTitle = QStringLiteral("1941年5月27日 · 北大西洋");
        n.musicKey      = QStringLiteral("wolfpack");
        n.type          = NodeType::Narrative;
        n.text =
            QStringLiteral("鱼雷炸断了俾斯麦号的舵机，庞大的战舰只能在大西洋上打转。\n\n"
            "英国的军舰从四面八方围拢来。炮弹不断落在甲板上，上层建筑已经全部被摧毁。\n"
            "大火在到处蔓延，死亡人数在攀升。\n\n"
            "「这艘船打不沉，」有人在说，「最强大的战列舰。」\n\n"
            "但人打得沉。甲板上已经铺满了死亡和呻吟。\n\n"
            "「准备弃船。」命令下达。");
        n.nextNodeId = QStringLiteral("wp_bismarck_sinking");
        addNode(n);
    }

    // ========================================================
    // 节点 14: 深水炸弹 (战斗)
    // ========================================================
    {
        StoryNode n;
        n.id            = QStringLiteral("wp_depth_charge");
        n.locationTitle = QStringLiteral("1941年 · 北大西洋深处");
        n.musicKey      = QStringLiteral("wolfpack_deep");
        n.type          = NodeType::Choice;
        n.text =
            QStringLiteral("深水炸弹。\n\n"
            "爆炸的冲击波透过舱壁，把整个潜艇当作鼓面拍打。\n"
            "灯泡在振动中碎裂，舱内只剩下红色的备用灯光。\n"
            "有人在角落里呕吐——那不是胆怯，是内耳在极度压力变化下的正常反应。\n\n"
            "「距离！」声纳员喘着气，「上面有两艘护卫舰！」\n\n"
            "爆炸越来越近。你需要做出决定。");

        Choice c1;
        c1.text             = QStringLiteral("【极速下潜】继续下潜到三百米，超越设计极限");
        c1.isCombat         = true;
        c1.combatThreshold  = 55;
        c1.bonusClasses     = {PlayerClass::SubmarineCrew};
        c1.successNodeId    = QStringLiteral("wp_escaped");
        c1.failureNodeId    = QStringLiteral("wp_pressure_breach");
        c1.failHpDelta      = -35;
        c1.failMoraleDelta  = -20;

        Choice c2;
        c2.text             = QStringLiteral("【静默潜伏】保持二百米，关闭所有非必要设备，静待护卫舰离去");
        c2.isCombat         = true;
        c2.combatThreshold  = 45;
        c2.bonusClasses     = {PlayerClass::SubmarineCrew};
        c2.successNodeId    = QStringLiteral("wp_escaped");
        c2.failureNodeId    = QStringLiteral("wp_direct_hit");
        c2.failHpDelta      = -45;
        c2.failMoraleDelta  = -25;

        n.choices = {c1, c2};
        addNode(n);
    }

    // ========================================================
    // 节点 15: 俾斯麦沉没
    // ========================================================
    {
        StoryNode n;
        n.id            = QStringLiteral("wp_bismarck_sinking");
        n.locationTitle = QStringLiteral("1941年5月27日早晨 · 北大西洋");
        n.musicKey      = QStringLiteral("wolfpack");
        n.type          = NodeType::Choice;
        n.text =
            QStringLiteral("「弃船！所有人弃船！」\n\n"
            "你跳入大西洋。水是冰冷的，冷得像刀。\n\n"
            "俾斯麦号在爆炸和炮火中翻转，那巨大的舰体以惊人的速度沉入海中。\n"
            "你看着它消失，只剩下海面上漂浮的碎片、油污、和挣扎的人。\n\n"
            "英国军舰靠近了。他们放下救生艇。\n"
            "但当德国潜艇的警报出现时，英国人紧急撤离——\n"
            "留下了还在水里的你和数百名同伴。\n\n"
            "水温：十四度。你能在这里坚持多久？");

        Choice c1;
        c1.text             = QStringLiteral("【等待】继续等待英国救援船回来");
        c1.isCombat         = true;
        c1.combatThreshold  = 50;
        c1.bonusClasses     = {PlayerClass::BattleshipCrew};
        c1.successNodeId    = QStringLiteral("wp_rescued");
        c1.failureNodeId    = QStringLiteral("wp_drowning");
        c1.failHpDelta      = -40;
        c1.failMoraleDelta  = -20;

        Choice c2;
        c2.text             = QStringLiteral("【自救】抓住浮木，努力向最近的漂浮物游去");
        c2.nextNodeId       = QStringLiteral("wp_rescued");
        c2.hpDelta          = -20;
        c2.moraleDelta      = -10;

        n.choices = {c1, c2};
        addNode(n);
    }

    // ========================================================
    // 节点 16: 逃脱
    // ========================================================
    {
        StoryNode n;
        n.id            = QStringLiteral("wp_escaped");
        n.locationTitle = QStringLiteral("1941年 · 北大西洋，深处");
        n.musicKey      = QStringLiteral("wolfpack_deep");
        n.type          = NodeType::Narrative;
        n.text =
            QStringLiteral("护卫舰的螺旋桨声越来越远，最后消失了。\n\n"
            "艇内静得可以听见彼此的心跳。\n\n"
            "「他们走了，」声纳员的声音在颤抖，「他们走了。」\n\n"
            "有人开始哭泣，不是软弱，是因为活着而感到一种巨大的、难以承受的感激。\n\n"
            "你把手放在舱壁上，感受着铁的冰凉，感受着艇体的颤动——\n"
            "你们还活着。");
        n.nextNodeId = QStringLiteral("wp_return_base");
        addNode(n);
    }

    // ========================================================
    // 节点 17: 压力突破
    // ========================================================
    {
        StoryNode n;
        n.id            = QStringLiteral("wp_pressure_breach");
        n.locationTitle = QStringLiteral("1941年 · 北大西洋深处");
        n.musicKey      = QStringLiteral("wolfpack_deep");
        n.type          = NodeType::Narrative;
        n.text =
            QStringLiteral("舱壁的某处开始漏水。\n\n"
            "不是大漏，只是一条细线，但在这个深度，每一滴海水都代表着巨大的压力。\n\n"
            "「锤住它！」工程兵用橡皮锤拼命击打泄漏点。\n\n"
            "十分钟后，漏水减缓了，但没有完全止住。\n\n"
            "你命令上浮。深水炸弹仍然在外面爆炸，但你们没有选择了。");
        n.nextNodeId = QStringLiteral("wp_escaped");
        addNode(n);
    }

    // ========================================================
    // 节点 18: 直接命中
    // ========================================================
    {
        StoryNode n;
        n.id            = QStringLiteral("wp_direct_hit");
        n.locationTitle = QStringLiteral("1941年 · 北大西洋");
        n.musicKey      = QStringLiteral("wolfpack_deep");
        n.type          = NodeType::Narrative;
        n.text =
            QStringLiteral("深水炸弹正中潜艇。\n\n"
            "爆炸的冲击力将你掀倒，头撞在仪表台上，眼前一片发黑。\n\n"
            "当你醒来时，舱内已经在进水。声纳员已经不动了，他的头部流着血。\n\n"
            "「紧急上浮！」你吼道。\n\n"
            "艇身开始颤抖上升，像一条受伤的鱼试图浮出水面。");
        n.nextNodeId = QStringLiteral("wp_return_base");
        addNode(n);
    }

    // ========================================================
    // 节点 19: 获救
    // ========================================================
    {
        StoryNode n;
        n.id            = QStringLiteral("wp_rescued");
        n.locationTitle = QStringLiteral("1941年5月27日 · 北大西洋");
        n.musicKey      = QStringLiteral("wolfpack");
        n.type          = NodeType::Narrative;
        n.text =
            QStringLiteral("一艘英国轻巡洋舰回来了，冒险靠近。\n\n"
            "英国水手从船舷上伸下绳梯，拉起一个又一个在水里的人。\n\n"
            "当你被拉上甲板时，一个英国水兵把毯子裹在你身上，\n"
            "他的眼神里没有仇恨，只有某种疲惫的同情。\n\n"
            "俾斯麦号的最后幸存者：两千二百二十二人中，只有一百一十五人获救。\n\n"
            "那一天死去的人，有一半是因为英国人被潜艇警报吓走，来不及救援。\n"
            "战争让每一个人都做出了他们本不愿做的事。");
        n.nextNodeId = QStringLiteral("wp_return_base");
        addNode(n);
    }

    // ========================================================
    // 节点 20: 溺亡
    // ========================================================
    {
        StoryNode n;
        n.id            = QStringLiteral("wp_drowning");
        n.locationTitle = QStringLiteral("1941年 · 北大西洋");
        n.musicKey      = QStringLiteral("wolfpack_deep");
        n.type          = NodeType::Narrative;
        n.text =
            QStringLiteral("水温把你的力气一点一点抽走。\n\n"
            "你想起了汉堡的港口，父亲的渔船，以及回家时家里那盏灯的颜色。\n\n"
            "大西洋很大，很冷。\n"
            "「爸，」你的嘴里说出这个字，然后是海水的苦涩。");
        n.nextNodeId = QStringLiteral("wp_defeat");
        addNode(n);
    }

    // ========================================================
    // 节点 21: 返回基地
    // ========================================================
    {
        StoryNode n;
        n.id            = QStringLiteral("wp_return_base");
        n.locationTitle = QStringLiteral("1941年-1943年 · 大西洋，岁月");
        n.musicKey      = QStringLiteral("wolfpack");
        n.type          = NodeType::Narrative;
        n.text =
            QStringLiteral("回到基地的那天，基尔港的海风还是一样咸。\n\n"
            "你在大西洋上交战的这些年里，德国潜艇部队击沉了两千六百多艘盟军商船，\n"
            "数以万计的商船水手随之沉入海底。\n\n"
            "但盟军的反潜技术越来越成熟。密码被破译，雷达越来越精准。\n"
            "到了1943年，德国潜艇的损失率已经高到无法承受——\n"
            "每两艘出发的潜艇，只有一艘能够回来。\n\n"
            "你和你的艇员们——还活着。许多人没有。\n\n"
            "「艇长，」大副站在跳板上，「我们回家了。」");
        n.nextNodeId = QStringLiteral("wp_final_reflection");
        addNode(n);
    }

    // ========================================================
    // 节点 22: 最终反思 (选择)
    // ========================================================
    {
        StoryNode n;
        n.id            = QStringLiteral("wp_final_reflection");
        n.locationTitle = QStringLiteral("1943年 · 法国洛里昂海军基地");
        n.musicKey      = QStringLiteral("wolfpack");
        n.type          = NodeType::Choice;
        n.text =
            QStringLiteral("基地的公告栏上贴着最新的损失名单。\n\n"
            "你扫视那些名字：海因茨·穆勒，弗里德里希·克劳斯，\n"
            "君特·布劳恩……都是你认识的人，或者认识认识他们的人。\n\n"
            "U艇部队从最初的灵感勃发，变成了一场骇人的消耗。\n"
            "在大西洋战役结束时，约三万两千名德国潜艇兵阵亡，\n"
            "占总兵力的四分之三。\n\n"
            "你写了一封给家里的信，停下来，不知道怎么写。");

        Choice c1;
        c1.text         = QStringLiteral("【诚实】如实描述大海的黑暗，不包装成荣耀");
        c1.nextNodeId   = QStringLiteral("wp_victory_honest");
        c1.moraleDelta  = 10;
        c1.grantedFlags = {QStringLiteral("wrote_truth")};

        Choice c2;
        c2.text         = QStringLiteral("【沉默】把信纸折起来，什么也不写");
        c2.nextNodeId   = QStringLiteral("wp_victory_silence");

        n.choices = {c1, c2};
        addNode(n);
    }

    // ========================================================
    // 节点 23: 胜利 — 诚实
    // ========================================================
    {
        StoryNode n;
        n.id            = QStringLiteral("wp_victory_honest");
        n.locationTitle = QStringLiteral("群狼海战 — 完成");
        n.musicKey      = QStringLiteral("wolfpack_triumph");
        n.type          = NodeType::Ending;
        n.isVictory     = true;
        n.text =
            QStringLiteral("【战役结束 — 生存】\n\n"
            "你在信里写道：\n\n"
            "「这里很冷，很暗，很安静。我击沉了一些船，也失去了一些战友。\n"
            "我不知道那些商船水手是否有妻儿，我想他们有的。\n"
            "我希望有一天，我能忘记他们的声音。」\n\n"
            "大西洋战役中，德国潜艇部队共击沉约二千六百艘盟军船只，\n"
            "造成约三万名商船水手死亡。\n\n"
            "作为报复，或者说，为了终结这一切，\n"
            "约三万名德国U艇艇员长眠于大西洋的海底。\n\n"
            "谁赢了这场战役？大海是唯一的答案。\n\n"
            "下一章即将开启。");
        addNode(n);
    }

    // ========================================================
    // 节点 24: 胜利 — 沉默
    // ========================================================
    {
        StoryNode n;
        n.id            = QStringLiteral("wp_victory_silence");
        n.locationTitle = QStringLiteral("群狼海战 — 完成");
        n.musicKey      = QStringLiteral("wolfpack");
        n.type          = NodeType::Ending;
        n.isVictory     = true;
        n.text =
            QStringLiteral("【战役结束 — 生存】\n\n"
            "你折好信纸，放回口袋。\n\n"
            "有些事，没有语言能够装下。\n\n"
            "大西洋战役，德德潜艇的「快乐时代」和最终的溃败。\n"
            "约三万名德国U艇艇员与约三万名盟军商船水手，\n"
            "用彼此的生命在大西洋底写下了同一个词：牺牲。\n\n"
            "区别只在于，有一方知道自己为何而死，另一方是——不确定的。\n\n"
            "下一章即将开启。");
        addNode(n);
    }

    // ========================================================
    // 节点 25: 失败
    // ========================================================
    {
        StoryNode n;
        n.id            = QStringLiteral("wp_defeat");
        n.locationTitle = QStringLiteral("群狼海战 — 失败");
        n.musicKey      = QStringLiteral("defeat_theme");
        n.type          = NodeType::Ending;
        n.isDefeat      = true;
        n.text =
            QStringLiteral("【战役结束 — 阵亡】\n\n"
            "大西洋吞下了你，就像它吞下了数以万计的其他人一样。\n\n"
            "那片深海没有墓碑，没有名字，只有黑暗和寒冷。\n\n"
            "你的名字将出现在基地的公告栏上，\n"
            "在那里待三天，然后被另一张公告覆盖。\n\n"
            "战争继续。大海继续。");
        addNode(n);
    }
}
