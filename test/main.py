from manim import *

# Python Union-Find 자료구조 (Manim 시각화와 별개로 내부 로직 관리)
class DisjointSetUnion:
    def __init__(self, n):
        self.parent = list(range(n))
        self.rank = [0] * n
        self.n = n
        self.history = [] # 연산 기록 (시각화를 위해)

    def find(self, i, record_history=True):
        # 경로 압축을 위한 경로 기록
        path_to_root_nodes = []
        current = i
        while current != self.parent[current]:
            path_to_root_nodes.append(current)
            current = self.parent[current]
        root = current
        
        # 경로 압축 실행
        compressed_parents = []
        for node_idx in path_to_root_nodes:
            if self.parent[node_idx] != root: # 이미 루트를 가리키지 않는 경우만 기록
                compressed_parents.append((node_idx, self.parent[node_idx], root)) # (노드, 이전 부모, 새 부모=루트)
            self.parent[node_idx] = root

        if record_history and compressed_parents:
             self.history.append({'type': 'path_compression', 'element': i, 'root': root, 'compressed_parents': compressed_parents})
        elif record_history and not compressed_parents and i != root : # 이미 압축된 경로를 다시 find 할 때
            self.history.append({'type': 'find_on_compressed_path', 'element': i, 'root': root, 'path': path_to_root_nodes + [root]})


        return root

    def union(self, i, j, record_history=True):
        if record_history:
            self.history.append({'type': 'union_start', 'elements': (i, j)})

        root_i = self.find(i, record_history=False)
        root_j = self.find(j, record_history=False)

        if record_history:
            self.history.append({'type': 'find_for_union', 'element_i': i, 'root_i': root_i, 'element_j': j, 'root_j': root_j})

        if root_i != root_j:
            op_details = {
                'type': 'union_action', 
                'elements': (i, j), # <--- 이 부분을 추가 또는 'i', 'j'와 함께 유지
                'i': i, 
                'j': j, 
                'root_i': root_i, 
                'root_j': root_j, 
                'rank_i': self.rank[root_i], 
                'rank_j': self.rank[root_j]
            }
            if self.rank[root_i] < self.rank[root_j]:
                self.parent[root_i] = root_j
                op_details.update({'merged_to': root_j, 'new_parent_of': root_i})
            elif self.rank[root_i] > self.rank[root_j]:
                self.parent[root_j] = root_i
                op_details.update({'merged_to': root_i, 'new_parent_of': root_j})
            else:
                self.parent[root_j] = root_i
                self.rank[root_i] += 1
                op_details.update({'merged_to': root_i, 'new_parent_of': root_j, 'rank_updated_for': root_i, 'new_rank': self.rank[root_i]})
            
            if record_history:
                self.history.append(op_details)
            return True
        else:
            if record_history:
                 self.history.append({'type': 'union_noop', 'elements': (i,j), 'root': root_i, 'message': 'Already in same set'})
            return False

class UnionFindExplanation(Scene):
    def setup(self):
        self.num_elements = 6
        self.dsu = DisjointSetUnion(self.num_elements)

        self.nodes_mob = VGroup()
        self.node_labels_mob = VGroup()
        self.arrows_mob = VGroup() # {node_idx: arrow_object}
        self.parent_array_mob = VGroup()
        self.rank_array_mob = VGroup()

        # 원형 배치
        radius = 2.8
        self.node_positions = [
            radius * np.array([np.cos(theta), np.sin(theta), 0]) + UP * 0.2
            for theta in np.linspace(TAU/self.num_elements * 0.5, TAU * (self.num_elements - 0.5)/self.num_elements, self.num_elements)
        ]
        
        self.current_op_text = Text("", font_size=30).to_edge(DOWN, buff=0.5)
        self.add(self.current_op_text)
        
        # Animation config
        self.node_radius = 0.4
        self.arrow_color = YELLOW
        self.highlight_color_find = ORANGE
        self.highlight_color_path_compress = GREEN
        self.highlight_color_union_root1 = PURE_BLUE
        self.highlight_color_union_root2 = PURE_RED
        self.default_node_color = BLUE

    def construct(self):
        title = Text("Union-Find Algorithm Explained", font_size=48).to_edge(UP)
        self.play(Write(title))
        self.wait(2)
        self.remove(title)

        # 1. 초기 상태
        self.show_text("1. Initial State: Each element is its own parent (root). Rank is 0.", 2)
        self.create_initial_visuals()
        self.wait(2)

        # 2. Union(0, 1)
        self.show_text("2. Union(0, 1)", 2)
        self.perform_union_animation(0, 1)
        self.wait(2)

        # 3. Union(2, 3)
        self.show_text("3. Union(2, 3)", 2)
        self.perform_union_animation(2, 3)
        self.wait(2)

        # 4. Union(0, 2) - 다른 랭크의 집합 합치기
        self.show_text("4. Union(0, 2) - Ranks are same. Merge 2's set into 0's. Increment rank of 0.", 2.5)
        self.perform_union_animation(0, 2) # 0-1 / 2-3
        self.wait(2)

        # 5. Union(4, 5)
        self.show_text("5. Union(4, 5)", 2)
        self.perform_union_animation(4, 5)
        self.wait(2)
        
        # 6. Find(3) - 경로 압축 시연
        self.show_text("6. Find(3) - Demonstrating Path Compression", 2)
        self.perform_find_animation(3) # 3 -> 2 -> 0 (0 is root)
        self.wait(2)

        # 7. Union(3, 4) - 압축된 경로의 노드와 다른 집합 합치기
        self.show_text("7. Union(3, 4) - Node 3's path is compressed. Union with 4's set.", 2.5)
        # 현재 0(rank 1, 0-1-2-3) 과 4(rank 0, 4-5)
        self.perform_union_animation(3, 4)
        self.wait(2)

        # 8. Find(5) - 다른 경로 압축 시연
        self.show_text("8. Find(5) - Path compression on another branch.", 2)
        self.perform_find_animation(5) # 5 -> 4 -> 0 (0 is root)
        self.wait(2)
        
        # 9. Union(1,5) - 이미 같은 집합
        self.show_text("9. Union(1, 5) - Elements are already in the same set.", 2)
        self.perform_union_animation(1, 5) # 이미 0을 루트로 같은 집합
        self.wait(3)

        self.show_text("End of Demonstration", 2)
        self.wait(3)

    def show_text(self, text_str, duration=1.5):
        new_text = Text(text_str, font_size=30).to_edge(DOWN, buff=0.5)
        self.play(Transform(self.current_op_text, new_text))
        self.wait(duration)

    def create_initial_visuals(self):
        # Nodes and Labels
        for i in range(self.num_elements):
            node = Circle(radius=self.node_radius, color=self.default_node_color, fill_opacity=0.7).move_to(self.node_positions[i])
            label = Text(str(i), font_size=24, color=WHITE).move_to(node.get_center())
            self.nodes_mob.add(node)
            self.node_labels_mob.add(label)
        self.play(LaggedStart(*[Create(n) for n in self.nodes_mob], lag_ratio=0.2))
        self.play(LaggedStart(*[Write(l) for l in self.node_labels_mob], lag_ratio=0.2))

        # Parent and Rank Arrays
        self.update_array_display(initial=True)
        # Arrows (initially none as parent[i] == i)
        self.arrows_mob = VGroup(*[VGroup() for _ in range(self.num_elements)]) # 각 노드별 화살표 그룹
        self.add(self.arrows_mob) # 빈 그룹 추가

    def update_array_display(self, initial=False, highlight_indices=None):
        if highlight_indices is None:
            highlight_indices = {}

        # Parent Array
        new_parent_array_mob = VGroup()
        p_header = Text("Parent:", font_size=24, weight=BOLD)
        p_cells = VGroup(*[
            Text(f"{self.dsu.parent[i]}", font_size=20).set_color(highlight_indices.get(f"p{i}", WHITE))
            for i in range(self.num_elements)
        ]).arrange(RIGHT, buff=0.3)
        
        # 수정된 부분: p_indices 정렬
        p_indices = VGroup(*[Text(f"{i}", font_size=18, color=GREY_A) for i in range(self.num_elements)])
        # 각 인덱스를 해당 셀 위에 위치시키기
        for idx, cell in zip(p_indices, p_cells):
            idx.next_to(cell, UP, buff=0.1) # 셀 위에 인덱스 위치

        new_parent_array_mob.add(p_header, p_indices, p_cells).arrange(DOWN, buff=0.15, aligned_edge=LEFT)
        new_parent_array_mob.to_corner(UL, buff=0.5)

        # Rank Array
        new_rank_array_mob = VGroup()
        r_header = Text("Rank:", font_size=24, weight=BOLD)
        r_cells = VGroup(*[
            Text(f"{self.dsu.rank[i]}", font_size=20).set_color(highlight_indices.get(f"r{i}", WHITE))
            for i in range(self.num_elements)
        ]).arrange(RIGHT, buff=0.3)
        
        # 수정된 부분: r_indices 정렬
        r_indices = VGroup(*[Text(f"{i}", font_size=18, color=GREY_A) for i in range(self.num_elements)])
        # 각 인덱스를 해당 셀 위에 위치시키기
        for idx, cell in zip(r_indices, r_cells):
            idx.next_to(cell, UP, buff=0.1) # 셀 위에 인덱스 위치

        new_rank_array_mob.add(r_header, r_indices, r_cells).arrange(DOWN, buff=0.15, aligned_edge=LEFT)
        new_rank_array_mob.next_to(new_parent_array_mob, DOWN, buff=0.4, aligned_edge=LEFT)

        if initial:
            self.parent_array_mob = new_parent_array_mob
            self.rank_array_mob = new_rank_array_mob
            self.play(Write(self.parent_array_mob), Write(self.rank_array_mob))
        else:
            self.play(
                Transform(self.parent_array_mob, new_parent_array_mob),
                Transform(self.rank_array_mob, new_rank_array_mob)
            )
        self.wait(0.5)

    def _draw_single_arrow(self, from_idx, to_idx, color=None):
        if color is None: color = self.arrow_color
        start_node_center = self.nodes_mob[from_idx].get_center()
        end_node_center = self.nodes_mob[to_idx].get_center()
        
        direction = normalize(end_node_center - start_node_center)
        # 화살표가 노드 경계에 닿도록 조정
        start_point = start_node_center + direction * self.node_radius
        end_point = end_node_center - direction * self.node_radius
        
        arrow = Arrow(start_point, end_point, buff=0, stroke_width=4, max_tip_length_to_length_ratio=0.2, color=color)
        return arrow

    def update_arrows_visuals(self, highlight_arrows=None): # highlight_arrows: list of (from_idx, to_idx, color)
        new_arrows_group = VGroup()
        current_arrows_map = {} # from_idx -> arrow_mob

        for i in range(self.num_elements):
            if self.dsu.parent[i] != i:
                arrow_color = self.arrow_color
                if highlight_arrows:
                    for h_from, h_to, h_color in highlight_arrows:
                        if i == h_from and self.dsu.parent[i] == h_to:
                            arrow_color = h_color
                            break
                
                new_arrow = self._draw_single_arrow(i, self.dsu.parent[i], color=arrow_color)
                new_arrows_group.add(new_arrow)
                current_arrows_map[i] = new_arrow

        # 이전 화살표 그룹과 비교하여 애니메이션 적용
        # self.arrows_mob는 VGroup으로 가정
        # 이 부분은 더 정교한 TransformMatchingShapes 등을 고려할 수 있으나, 여기서는 간단히 대체
        anims = []
        # 기존 화살표 제거
        if self.arrows_mob and len(self.arrows_mob) > 0:
            anims.append(FadeOut(self.arrows_mob)) # 이전 모든 화살표 FadeOut
        
        # 새 화살표 추가
        if len(new_arrows_group) > 0:
            anims.append(FadeIn(new_arrows_group))

        if anims:
            self.play(*anims, run_time=0.7)
        
        self.remove(self.arrows_mob) # 이전 화살표 그룹 제거
        self.arrows_mob = new_arrows_group # 새 화살표 그룹으로 교체
        self.add(self.arrows_mob) # 씬에 추가 (FadeIn 했으므로 이미 존재할 수 있음)

        self.wait(0.5)

    def perform_find_animation(self, element_idx):
        self.show_text(f"Find({element_idx}): Finding root...", 0.5)

        # 1. 경로 추적 애니메이션
        path_nodes_indices = []
        path_arrows_to_highlight = [] # (from, to, color)
        
        curr = element_idx
        animations = [self.nodes_mob[curr].animate.set_fill(self.highlight_color_find, opacity=1.0)]
        self.play(*animations, run_time=0.5)
        self.wait(0.3)

        while curr != self.dsu.parent[curr]:
            path_nodes_indices.append(curr)
            parent_of_curr = self.dsu.parent[curr]
            
            # 현재 노드 -> 부모 노드로 이동하는 화살표 찾아서 하이라이트
            # (update_arrows_visuals에서 highlight_arrows 인자로 전달할 정보)
            path_arrows_to_highlight.append((curr, parent_of_curr, self.highlight_color_find))
            
            # 부모 노드 하이라이트
            animations = [
                self.nodes_mob[parent_of_curr].animate.set_fill(self.highlight_color_find, opacity=1.0)
            ]
            # 해당 화살표 임시로 색 변경 (update_arrows_visuals 호출 전에)
            # 주의: 이 방식은 self.arrows_mob를 직접 수정하므로, update_arrows_visuals가 이를 덮어씀
            # 더 나은 방법은 update_arrows_visuals에 highlight 정보를 전달하는 것
            # 여기서는 간단히 노드 색상 변경에 집중
            self.update_arrows_visuals(highlight_arrows=path_arrows_to_highlight) # 경로 추적하며 화살표 하이라이트
            self.play(*animations, run_time=0.7)
            self.wait(0.3)
            
            curr = parent_of_curr
        
        root_node_idx = curr
        path_nodes_indices.append(root_node_idx) # 루트도 경로에 포함

        self.show_text(f"Find({element_idx}): Root is {root_node_idx}. Path: {path_nodes_indices}", 1.5)
        self.wait(1)

        # 2. 경로 압축 애니메이션
        # 먼저 실제 DSU 객체에서 find를 호출하여 parent 배열을 업데이트 (압축 실행)
        original_parents_before_compress = list(self.dsu.parent) # 압축 전 부모 상태 저장
        self.dsu.find(element_idx, record_history=True) # 여기서 path_compression 히스토리 기록됨
        
        # 히스토리에서 압축 정보 가져오기 (가장 최근 path_compression 이벤트)
        compression_info = None
        for event in reversed(self.dsu.history):
            if event['type'] == 'path_compression' and event['element'] == element_idx:
                compression_info = event
                break
        
        if compression_info:
            self.show_text(f"Path Compression for Find({element_idx})...", 1)
            
            # 압축되는 노드들 색 변경
            compression_anims = []
            path_compress_arrows_highlight = [] # (from, new_to, color)
            
            for node_idx, old_parent_idx, new_parent_idx in compression_info['compressed_parents']:
                compression_anims.append(self.nodes_mob[node_idx].animate.set_fill(self.highlight_color_path_compress, opacity=1.0))
                # 새 부모를 가리키는 화살표를 하이라이트하기 위함
                path_compress_arrows_highlight.append((node_idx, new_parent_idx, self.highlight_color_path_compress))

            if compression_anims:
                 self.play(*compression_anims, run_time=0.7)
                 self.wait(0.5)

            # Parent 배열 업데이트 및 화살표 다시 그리기 (압축된 상태로)
            self.update_array_display(highlight_indices={f"p{data[0]}": self.highlight_color_path_compress for data in compression_info['compressed_parents']})
            self.update_arrows_visuals(highlight_arrows=path_compress_arrows_highlight) # 압축된 화살표 하이라이트

            self.show_text(f"Path for {element_idx} compressed to root {root_node_idx}.", 1.5)
            self.wait(1)
        else: # 이미 압축된 경우
            self.show_text(f"Path for {element_idx} was already compressed or it's the root.", 1.5)
            self.wait(1)


        # 3. 모든 하이라이트 해제
        reset_animations = [
            self.nodes_mob[idx].animate.set_fill(self.default_node_color, opacity=0.7) for idx in path_nodes_indices
        ]
        if compression_info: # 압축된 노드들도 원래 색으로
             reset_animations.extend([
                self.nodes_mob[data[0]].animate.set_fill(self.default_node_color, opacity=0.7) 
                for data in compression_info['compressed_parents']
            ])

        self.play(*reset_animations, run_time=0.7)
        self.update_arrows_visuals() # 모든 화살표 기본색으로
        self.wait(0.5)


    def perform_union_animation(self, el1_idx, el2_idx):
        self.show_text(f"Union({el1_idx}, {el2_idx}): Starting...", 0.5)

        # 1. Find(el1) & Find(el2) - 루트 찾기 (내부적으로 경로 압축 발생 가능)
        # 애니메이션 없이 dsu 내부 상태만 업데이트 (실제 압축은 find_animation에서 보여줌)
        root1 = self.dsu.find(el1_idx, record_history=False)
        root2 = self.dsu.find(el2_idx, record_history=False)
        
        # 루트 노드들 하이라이트
        find_anims = []
        if el1_idx == root1: # el1이 이미 루트인 경우
            find_anims.append(self.nodes_mob[el1_idx].animate.set_fill(self.highlight_color_union_root1))
        else:
            find_anims.extend([
                self.nodes_mob[el1_idx].animate.set_color(self.highlight_color_union_root1), # 찾는 노드 테두리 색 변경
                self.nodes_mob[root1].animate.set_fill(self.highlight_color_union_root1) # 루트 노드 채우기 색 변경
            ])

        if el2_idx == root2:
            find_anims.append(self.nodes_mob[el2_idx].animate.set_fill(self.highlight_color_union_root2))
        else:
             find_anims.extend([
                self.nodes_mob[el2_idx].animate.set_color(self.highlight_color_union_root2),
                self.nodes_mob[root2].animate.set_fill(self.highlight_color_union_root2)
            ])
        
        self.play(*find_anims, run_time=0.7)
        self.show_text(f"Roots: Find({el1_idx}) -> {root1}, Find({el2_idx}) -> {root2}", 1.5)
        self.wait(1)
        
        # 실제 DSU union 연산 (parent, rank 업데이트)
        # 히스토리 기록을 위해 여기서 union 호출
        self.dsu.union(el1_idx, el2_idx, record_history=True)
        
        # 히스토리에서 union 결과 가져오기
        union_event = None
        for event in reversed(self.dsu.history):
            if event['type'] == 'union_action':
                # union_action 이벤트는 'i'와 'j' 키를 가짐
                if event['i'] == el1_idx and event['j'] == el2_idx:
                    union_event = event
                    break
            elif event['type'] == 'union_noop':
                # union_noop 이벤트는 'elements' 키를 가짐
                if event['elements'] == (el1_idx, el2_idx):
                    union_event = event
                    break

        highlight_array_indices = {}
        highlight_arrow_info = [] # [(from, to, color)]

        if union_event and union_event['type'] == 'union_action':
            merged_to_root = union_event['merged_to']
            new_parent_of_root = union_event['new_parent_of']
            
            # 합쳐지는 루트 하이라이트 (색 유지 또는 변경)
            # new_parent_of_root가 merged_to_root를 가리키게 됨
            self.show_text(f"Merging set of {new_parent_of_root} into set of {merged_to_root}", 1)
            
            highlight_array_indices[f"p{new_parent_of_root}"] = GREEN # 변경된 parent 값
            highlight_arrow_info.append((new_parent_of_root, merged_to_root, GREEN))

            if 'rank_updated_for' in union_event:
                rank_updated_idx = union_event['rank_updated_for']
                highlight_array_indices[f"r{rank_updated_idx}"] = GREEN # 변경된 rank 값
                self.show_text(f"Rank of root {rank_updated_idx} increased to {union_event['new_rank']}", 1.5)
        
        elif union_event and union_event['type'] == 'union_noop':
            self.show_text(f"Union({el1_idx}, {el2_idx}): {union_event['message']}", 1.5)
        
        # 배열 및 화살표 업데이트
        self.update_array_display(highlight_indices=highlight_array_indices)
        self.update_arrows_visuals(highlight_arrows=highlight_arrow_info)
        self.wait(1)

        # 하이라이트 해제
        reset_anims = [
            self.nodes_mob[idx].animate.set_fill(self.default_node_color).set_color(self.default_node_color)
            for idx in [el1_idx, el2_idx, root1, root2] # 관련된 모든 노드
        ]
        self.play(*reset_anims, run_time=0.7)
        self.update_arrows_visuals() # 모든 화살표 기본색으로
        self.wait(0.5)

# 실행: manim -pql UnionFindExplanation.py UnionFindExplanation