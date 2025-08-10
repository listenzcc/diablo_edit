from nicegui import ui
from typing import List, Dict, Set
from dataclasses import dataclass


@dataclass
class ItemRecord:
    player_name: str
    item_name: str
    quality: int


QualityTable = {
    1: 'Low Quality',
    2: 'Normal',
    3: 'High quality',
    4: 'Magically',
    5: 'Set',
    6: 'Rare',
    7: 'Unique',
    8: 'Crafted'
}


def parse_quality(quality: int) -> str:
    if quality in QualityTable:
        return QualityTable[quality]

    if quality - 10 in QualityTable:
        return f'RuneWord({QualityTable[quality - 10]})'

    return 'Other'


def parse_item_file(file_path: str) -> List[ItemRecord]:
    records = []
    with open(file_path, 'r', encoding='utf-8') as f:
        current_record = {}
        for line in f:
            line = line.strip()
            if line.startswith('Player Name:'):
                current_record['player_name'] = line.split(': ')[1]
            elif line.startswith('Item Name:'):
                current_record['item_name'] = line.split(': ')[1]
            elif line.startswith('Quality:'):
                current_record['quality'] = int(line.split(': ')[1])
            elif line.startswith('------------------------------'):
                if current_record:
                    records.append(ItemRecord(**current_record))
                    current_record = {}
    return records


class ItemQueryApp:
    def __init__(self, file_path: str):
        self.file_path = file_path
        self.records = parse_item_file(file_path)
        self.available_qualities = sorted({r.quality for r in self.records})
        self.setup_ui()

    def setup_ui(self):
        # 共享的品质多选框
        with ui.card().classes('w-full'):
            ui.label('Quality Filter').classes('text-xl')
            self.quality_checkboxes = {}
            with ui.row().classes('w-full'):
                for quality in self.available_qualities:
                    self.quality_checkboxes[quality] = ui.checkbox(
                        text='{} | {}'.format(quality, parse_quality(quality)),
                        value=True,
                        on_change=self.update_all
                    )

        with ui.row():
            # Player to Items query
            with ui.card().classes('w-[600px]'):
                ui.label('Player to Items Query').classes('text-xl')
                self.player_select = ui.select(
                    options=sorted({r.player_name for r in self.records}),
                    label='Select Player',
                    on_change=self.update_player_items
                ).classes('w-full')

                columns = [
                    {'name': 'item_name', 'label': 'Item Name', 'field': 'item_name'},
                    {'name': 'quality', 'label': 'Quality', 'field': 'quality'}
                ]
                self.player_items_table = ui.table(
                    columns=columns, rows=[]).classes('w-full')

            # Item to Players query
            with ui.card().classes('w-[600px]'):
                ui.label('Item to Players Query').classes('text-xl')
                self.item_count_label = ui.label('N.A.')
                self.item_select = ui.select(
                    options=self.get_filtered_items(),  # 初始使用过滤后的物品列表
                    label='Select Item',
                    on_change=self.update_item_players
                ).classes('w-full')

                columns = [
                    {'name': 'player_name', 'label': 'Player Name',
                        'field': 'player_name'},
                    {'name': 'quality', 'label': 'Quality', 'field': 'quality'}
                ]
                self.item_players_table = ui.table(
                    columns=columns, rows=[]).classes('w-full')

        # 初始化时更新所有内容
        self.update_all()

    def get_selected_qualities(self) -> Set[int]:
        return {q for q, cb in self.quality_checkboxes.items() if cb.value}

    def get_filtered_items(self) -> List[str]:
        selected_qualities = self.get_selected_qualities()
        return sorted({
            r.item_name
            for r in self.records
            if r.quality in selected_qualities
        })

    def update_all(self):
        # 获取过滤后的物品列表
        filtered_items = self.get_filtered_items()

        # 更新物品选择框选项
        self.item_count_label.text = f'Select Item in {len(filtered_items)}'
        self.item_select.options = filtered_items

        # 如果当前选择的物品不在新选项中，清空选择
        if self.item_select.value and self.item_select.value not in filtered_items:
            self.item_select.value = None

        # 如果选项列表不为空且当前没有选择，设置第一个选项为默认值
        if filtered_items and not self.item_select.value:
            self.item_select.value = filtered_items[0]

        # 更新两个表格
        self.update_player_items()
        self.update_item_players()

    def update_player_items(self):
        player_name = self.player_select.value
        selected_qualities = self.get_selected_qualities()

        if not player_name:
            self.player_items_table.rows = []
        else:
            rows = [
                {'item_name': r.item_name, 'quality': r.quality}
                for r in self.records
                if r.player_name == player_name and r.quality in selected_qualities
            ]
            rows = sorted(rows, key=lambda e: e['quality'])
            self.player_items_table.rows = rows
        self.player_items_table.update()

    def update_item_players(self):
        item_name = self.item_select.value
        selected_qualities = self.get_selected_qualities()

        if not item_name:
            self.item_players_table.rows = []
        else:
            rows = [
                {'player_name': r.player_name, 'quality': r.quality}
                for r in self.records
                if r.item_name == item_name and r.quality in selected_qualities
            ]
            self.item_players_table.rows = rows
        self.item_players_table.update()


# 使用示例
app = ItemQueryApp('./x64/Release/character_items.txt')

ui.run(title='Diablo II Item Query', port=8080)
