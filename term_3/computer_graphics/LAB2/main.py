import tkinter as tk
from tkinter import ttk, filedialog, messagebox
from PIL import Image
import os

# TODO: указать как обхожу директорию в доках  
# TODO: посчитать процент сжатия

class ImageInfoApp:
    def __init__(self, root):
        self.root = root
        self.root.title("Image Information Reader")
        self.root.geometry("800x600")

        self.item_to_path = {}

        self.create_widgets()

    def create_widgets(self):
        frame = tk.Frame(self.root)
        frame.pack(pady=10)

        self.folder_path = tk.StringVar()
        folder_label = tk.Label(frame, text="Selected Folder:")
        folder_label.pack(side=tk.LEFT, padx=5)
        folder_entry = tk.Entry(frame, textvariable=self.folder_path, width=50)
        folder_entry.pack(side=tk.LEFT, padx=5)
        browse_button = tk.Button(frame, text="Browse", command=self.browse_folder)
        browse_button.pack(side=tk.LEFT, padx=5)
        scan_button = tk.Button(frame, text="Scan Images", command=self.scan_images)
        scan_button.pack(side=tk.LEFT, padx=5)

        columns = ("Filename", "Size (px)", "Resolution (dpi)", "Color Depth", "Compression")
        self.tree = ttk.Treeview(self.root, columns=columns, show="headings")
        for col in columns:
            self.tree.heading(col, text=col)
            self.tree.column(col, anchor=tk.CENTER)
        self.tree.pack(fill=tk.BOTH, expand=True)

        scrollbar_y = ttk.Scrollbar(self.tree, orient=tk.VERTICAL, command=self.tree.yview)
        self.tree.configure(yscroll=scrollbar_y.set)
        scrollbar_y.pack(side=tk.RIGHT, fill=tk.Y)

        scrollbar_x = ttk.Scrollbar(self.root, orient=tk.HORIZONTAL, command=self.tree.xview)
        self.tree.configure(xscroll=scrollbar_x.set)
        scrollbar_x.pack(side=tk.BOTTOM, fill=tk.X)

        self.tree.bind('<Double-1>', self.on_double_click)

    def browse_folder(self):
        folder_selected = filedialog.askdirectory()
        if folder_selected:
            self.folder_path.set(folder_selected)

    def scan_images(self):
        folder = self.folder_path.get()
        if not folder:
            messagebox.showwarning("No Folder Selected", "Please select a folder to scan.")
            return

        for item in self.tree.get_children():
            self.tree.delete(item)
        self.item_to_path.clear()

        self.process_images(folder)

    def process_images(self, folder):
        supported_formats = ('.jpg', '.jpeg', '.gif', '.tif', '.tiff', '.bmp', '.png', '.pcx')
        image_files = []
        for root_dir, dirs, files in os.walk(folder):
            for file in files:
                if file.lower().endswith(supported_formats):
                    image_files.append(os.path.join(root_dir, file))

        total_files = len(image_files)
        if total_files == 0:
            messagebox.showinfo("No Images Found", "No supported image files were found in the selected folder.")
            return

        for idx, file_path in enumerate(image_files):
            try:
                with Image.open(file_path) as img:
                    filename = os.path.basename(file_path)
                    size_px = f"{img.width} x {img.height}"
                    dpi = img.info.get('dpi', (0, 0))
                    resolution = f"{dpi[0]} x {dpi[1]}"
                    color_depth = self.get_color_depth(img)
                    compression = self.get_compression_type(img)

                    item_id = self.tree.insert('', 'end', values=(filename, size_px, resolution, color_depth, compression))
                    self.item_to_path[item_id] = file_path

            except Exception as e:
                print(f"Error processing {file_path}: {e}")

    def get_color_depth(self, img):
        mode_to_bpp = {
            "1": 1,
            "L": 8,
            "P": 8,
            "RGB": 24,
            "RGBA": 32,
            "CMYK": 32,
            "YCbCr": 24,
            "LAB": 24,
            "HSV": 24,
            "I": 32,
            "F": 32,
        }
        return f"{mode_to_bpp.get(img.mode, 'Unknown')} bits"

    def get_compression_type(self, image):
        compression = "Неизвестно"

        if image.format in ["JPEG", "JPG"]:
            if "progressive" in image.info and image.info["progressive"]:
                compression = "Прогрессивное"
            else:
                compression = "Базовое"

        elif image.format == "PNG":
            if image.info.get("compression", 0) == 0:
                compression = "Без потерь"
            elif image.info.get("compression", 0) == 1:
                compression = "Расширенный последовательный"

        elif image.format == "GIF":
            compression = "Без потерь"

        elif image.format == "TIFF":
            if "compression" in image.tag_v2:
                tiff_compression = image.tag_v2[256]
                if tiff_compression == 1:
                    compression = "Нет сжатия"
                elif tiff_compression == 2:
                    compression = "CCITTRLE"
                elif tiff_compression == 3:
                    compression = "CCITTFAX3"
                elif tiff_compression == 4:
                    compression = "CCITTFAX4"
                elif tiff_compression == 5:
                    compression = "LZW"
                elif tiff_compression == 32773:
                    compression = "PackBits"
                else:
                    compression = "Неизвестное сжатие"

        elif image.format == "BMP":
            compression = "Нет сжатия"

        elif image.format == "PCX":
            compression = "RLE"

        return compression

    def on_double_click(self, event):
        selected_item = self.tree.focus()
        if selected_item:
            file_path = self.item_to_path.get(selected_item)
            if file_path:
                try:
                    if sys.platform.startswith('win'):
                        os.startfile(file_path)
                    elif sys.platform.startswith('darwin'):
                        subprocess.call(['open', file_path])
                    elif sys.platform.startswith('linux'):
                        subprocess.call(['xdg-open', file_path])
                    else:
                        messagebox.showerror("Error", "Unsupported operating system.")
                except Exception as e:
                    messagebox.showerror("Error", f"Cannot open file: {e}")

if __name__ == "__main__":
    root = tk.Tk()
    app = ImageInfoApp(root)
    root.mainloop()
