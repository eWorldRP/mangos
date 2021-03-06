-- Arathi Basin
DELETE FROM `achievement_criteria_requirement` WHERE criteria_id IN (123,308,414,426,427,431,432,433,434,435,1237,3372,6641,6642,6643,6644);
INSERT INTO `achievement_criteria_requirement` (`criteria_id`, `type`, `value1`, `value2`) VALUES
    ('123','6','3358','0'),
    ('308','6','3358','0'),
    ('414','6','3358','0'),
    ('426','6','3358','0'),
    ('427','6','3358','0'),
    ('1237','6','3358','0'),
    ('3372','6','3358','0'),
    ('6641','6','3820','0'),
    ('6642','6','3358','0'),
    ('6643','6','2597','0'),
    ('6644','6','3277','0');

-- Warsong Gulch
DELETE FROM `achievement_criteria_requirement` WHERE `criteria_id` IN (416,417,1801,1802,1824,3880,5605,5606);
INSERT INTO `achievement_criteria_requirement` (`criteria_id`, `type`, `value1`, `value2`) VALUES
    ('416','6','3277','0'),
    ('417','6','3277','0'),
    ('1801','6','3277','0'),
    ('1802','6','3277','0'),
    ('1824','6','3277','0'),
    ('3880','6','3277','0'),
    ('3880','7','23451','0'),
    ('5605','6','4571','0'),
    ('5605','7','23333','0'),
    ('5606','6','4572','0'),
    ('5606','7','23335','0');

-- Eye of the Storm
DELETE FROM `achievement_criteria_requirement` WHERE `criteria_id` IN (418,441,442,443,444,3685);
INSERT INTO `achievement_criteria_requirement` (`criteria_id`, `type`, `value1`, `value2`) VALUES
    ('418','6','3820','0'),
    ('441','6','3870','0'),
    ('442','6','3871','0'),
    ('443','6','3872','0'),
    ('444','6','3869','0'),
    ('3685','7','34976','0');