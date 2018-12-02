
#import component
from component import *

class Dot :
	def __init__(self, dot_file) :
		self._dot_file = dot_file
		self._component_pairs = []
		self._all_components = []

	def parse_dot(self) :
		'''
		parse Esmeralda.dot and return an object that called
		ComponentPyramid which contains several levels, and 
		each level contains serval components
		'''
		print("Parsing dot file : " + self._dot_file)

		trans_table = str.maketrans({'"':None , ';':None}) 

		# Generate the component_pairs
		with open(self._dot_file) as df :
			for line in df :
				if "->" in line :
					line = line.translate(trans_table)

					left = line.partition('->')[0].strip()
					right = line.partition('->')[2].strip()

					dependent = Component(*left.split('@'))
					dependency = Component(*right.split('@'))

					self._component_pairs += [[dependent, dependency]] 

		# Generate all_components
		for pair in self._component_pairs :
			# Deal with dependent
			index = self._index_in_all_comonents(pair[0].name)

			if index >= 0 :		# already in all components
				self._all_components[index].add_dependency(pair[1].name)
				dependent_level = self._all_components[index].level	# save the dependent's level as the dependency may use it
			else :		# not in all components yet
				component = ExpandedComponent(pair[0].name, pair[0].tag)
				component.add_dependency(pair[1].name)
				self._insert_component_to_all_components(component)
				dependent_level = component.level

			# Deal with dependency
			index = self._index_in_all_comonents(pair[1].name)

			if index >= 0 :		# already in all components
				self._all_components[index].level = dependent_level + 1
			else :		# not in all components yet
				component = ExpandedComponent(pair[1].name, pair[1].tag)
				component.level = dependent_level + 1
				self._insert_component_to_all_components(component)

	@property
	def component_pairs(self) :
		for pairs in self._component_pairs :
			print("{} -> {}".format(pairs[0], pairs[1]))

	@property
	def component_names(self) :
		component_names = []
		for c in self._all_components :
			component_names.append(c.name)

		return component_names

	''' The same component will be only inserted once '''
	def _insert_component_to_all_components(self, expanded_component) :
		self._all_components.append(expanded_component)
		
	def _index_in_all_comonents(self, component_name) :
		index = 0
		for component in self._all_components :
			if component.name == component_name :
				return index
			index = index + 1

		return -1

	@property
	def all_components_in_reverse_order(self) :
		return sorted(self._all_components, key=ExpandedComponent.level_for_sort, reverse=True)
	
