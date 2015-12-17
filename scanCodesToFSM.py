#!python

import sys

USBCodes = {}
PS2Make = {}
PS2Break = {}

USBModifiers = {
	'KEY_LALT':   'MODIFIERKEY_LEFT_ALT',
	'KEY_LCTRL':  'MODIFIERKEY_LEFT_CTRL',
	'KEY_LGUI':   'MODIFIERKEY_LEFT_GUI',
	'KEY_LSHIFT': 'MODIFIERKEY_LEFT_SHIFT',
	'KEY_RALT':   'MODIFIERKEY_RIGHT_ALT',
	'KEY_RCTRL':  'MODIFIERKEY_RIGHT_CTRL',
	'KEY_RGUI':   'MODIFIERKEY_RIGHT_GUI',
	'KEY_RSHIFT': 'MODIFIERKEY_RIGHT_SHIFT',
}

def ps2List(str):
	if str == '-NONE-':
		# Weird special case for pause key
		return None

	seq = [int(x, 16) for x in str.split(',')]

	return seq

def addToDict(dictionary, what, keyName, info):
	if keyName in dictionary:
		print("%s: duplicate %s" % (keyName, what))
	else:
		# print("%s: %s %s" % (keyName, what, info))
		dictionary[keyName] = info

for line in sys.stdin:
	line = line.strip()

	fields = line.split(' // ')

	keyName = fields[0]

	if len(fields) == 2:
		if fields[1].startswith('USB '):
			usbCode = None

			try:
				usbCode = int(fields[1][4:], 16)
			except ValueError:
				print("%s: invalid USB code %s" % (keyName, code))

			if usbCode is not None:
				addToDict(USBCodes, 'USBCode', keyName, usbCode)
		else:
			print("%s: what does %s mean?" % (keyName, code))
	elif len(fields) == 3:
		makeSequence = None
		breakSequence = None

		try:
			makeSequence = ps2List(fields[1])
		except ValueError:
			print("%s: invalid make sequence %s" % (keyName, fields[1]))

		try:
			breakSequence = ps2List(fields[2])
		except ValueError:
			print("%s: invalid make sequence %s" % (keyName, fields[2]))

		if makeSequence is not None:
			addToDict(PS2Make, 'PS/2 make sequence', keyName, makeSequence)

		if breakSequence is not None:
			addToDict(PS2Break, 'PS/2 break sequence', keyName, breakSequence)
	else:
		print("???: %s" % line)

MAKE =     0x0100
BREAK =    0x0200
MODIFIER = 0x0400

class FSMNode (object):
	def __init__(self):
		self.entries = [ 0 for x in range(256) ]

	def __contains__(self, item):
		return self.entries[item] != 0

	def __setitem__(self, item, value):
		self.entries[item] = value

	def __getitem__(self, item):
		return self.entries[item]

	def dump(self):
		i = 0
		print("  {")

		while i < 256:
			sys.stdout.write("    /* 0x%02X - 0x%02X */" % (i, i + 7))

			for j in range(8):
				sys.stdout.write(" 0x%04X," % self[i])
				i += 1

			sys.stdout.write("\n")

		print("  },")

class FSM (object):
	def __init__(self):
		# The first node is all empty, at the moment
		self.states = []
		self.maxState = -1
		self.newNode()

	def newNode(self):
		self.states.append(FSMNode())
		self.maxState = len(self.states) - 1

		return self.maxState

	def locate(self, keyName, seq):
		state = 0
		nextState = None

		while seq:
			node = self.states[state]
			item = seq.pop(0)

			if not item in node:
				# print("%d - no %d for %s" % (state, item, keyName))

				# Push back the missing item
				seq.insert(0, item)

				return ( False, state, None, seq)

			nextState = node[item]

			# print("%d - %d => %04X for %s" % (state, item, nextState, keyName))

			if seq:
				if ((nextState & MAKE) or (nextState & BREAK)):
					# Push back the missing item
					seq.insert(0, item)

					# print("%d - %d for %s marked as terminal but: %s" % (state, item, keyName, seq))
					return ( False, -1, None, seq )

				state = nextState

		rc = ( True, state, nextState, None )

		# print("%d for %s - final %s" % (state, keyName, rc))

		return rc

	def insert(self, keyName, seq, terminal):
		found, state, nextState, remaining = self.locate(keyName, seq)

		if found:
			# print("%s: %s already present as %d => %04X?" % (keyName, seq, state, nextState))
			return

		# Not found, so off we go.
		while remaining:
			node = self.states[state]
			item = remaining.pop(0)

			if remaining:
				# Intermediate node.
				nextState = self.newNode()
				node[item] = nextState

				# print("%s: inserted state %d for %d - %d" % (keyName, nextState, state, item))
				state = nextState
			else:
				# Terminal.
				node[item] = terminal
				# print("%s: inserted terminal %04X for %d - %d" % (keyName, terminal, state, item))

	def dump(self):
		state = 0

		print("int keyFSM[%d][256] = {" % (self.maxState + 1));

		while state <= self.maxState:
			print("")
			print("// STATE %d" % state)
			self.states[state].dump()
			state += 1

		print("};")

fsm = FSM()

for keyName in sorted(PS2Make.keys()):
	# First, do we have a USB code for this?
	if keyName not in USBCodes:
		# Nope. Toss it.
		print("%s: skipping, no USB code" % keyName)
		continue

	usbCode = USBCodes[keyName]

	seq = PS2Make[keyName]

	if keyName in USBModifiers:
		usbCode |= MODIFIER

	fsm.insert(keyName, seq, MAKE | usbCode)

	# If it has a break code, do that now too.
	if keyName in PS2Break:
		seq = PS2Break[keyName]

		if keyName in USBModifiers:
			usbCode |= MODIFIER

		fsm.insert(keyName, seq, BREAK | usbCode)

fsm.dump()
