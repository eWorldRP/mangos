DELETE FROM `command` WHERE `name` IN ('chatspy set', 'chatspy reset', 'chatspy status', 'chatspy cancel');
INSERT INTO `command` VALUES
('chatspy set', 12, 'Syntax: .chatspy set $PlayerName\r\n Append ChatSpy on $player.'),
('chatspy reset', 12, 'Syntax: .chatspy reset\r\n Reset all ChatSpys.'),
('chatspy status', 12, 'Syntax: .chatspy status $PlayerName\r\n Show ChatSpy status for $player.'),
('chatspy cancel', 12, 'Syntax: .chatspy cancel $PlayerName\r\n Reset ChatSpy for $player.');