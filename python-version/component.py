
import re

class Tag :
	__TAG_RE = "(\d+\.){3,4}\d+"

	def __init__(self, tag) :
		self._tag = tag
		self._tag_re = re.compile(Tag.__TAG_RE)
		assert self._tag_re.fullmatch(tag)

class Component :
	def __init__(self, name, tag) :
		self._name = name
		self._tag = tag

	def __eq__(self, other) :
		return self._name == other._name

	def __str__(self) :
		return self._name

	@property
	def name(self) :
		return _name

	@property
	def tag(self) :
		return _tag

class ExpandedComponent(Component) :
	def __init__(self, name, tag) :
		super().__init__(name, tag)

		_level = 0
		self._dependencies = []

	def add_dependency(self, name) :
		self._dependencies.append(name)

	@property
	def level(self) :
		return _level

	# This is used for get the level of expanded_component as the key in sort method
	def level_for_sort(expanded_component) :
		return expanded_component.level

	@level.setter
	def level(self, new_level) :
		_level = new_level
