#include "stdafx.h"
#include "FilesP2PTransport.h"


//some additional functions only for this class

std::vector<char> load_file(std::string const& filename)
{
	std::fstream in;
	in.exceptions(std::ifstream::failbit);
	in.open(filename.c_str(), std::ios_base::in | std::ios_base::binary);
	in.seekg(0, std::ios_base::end);
	size_t const size = size_t(in.tellg());
	in.seekg(0, std::ios_base::beg);
	std::vector<char> ret(size);
	in.read(ret.data(), ret.size());
	return ret;
}

std::string branch_path(std::string const& f)
{
	if (f.empty()) return f;

#ifdef TORRENT_WINDOWS
	if (f == "\\\\") return "";
#endif
	if (f == "/") return "";

	int len = int(f.size());
	// if the last character is / or \ ignore it
	if (f[len - 1] == '/' || f[len - 1] == '\\') --len;
	while (len > 0) {
		--len;
		if (f[len] == '/' || f[len] == '\\')
			break;
	}

	if (f[len] == '/' || f[len] == '\\') ++len;
	return std::string(f.c_str(), len);
}

// do not include files and folders whose
// name starts with a .
bool file_filter(std::string const& f)
{
	if (f.empty()) return false;

	char const* first = f.c_str();
	char const* sep = strrchr(first, '/');
#if defined(TORRENT_WINDOWS) || defined(TORRENT_OS2)
	char const* altsep = strrchr(first, '\\');
	if (sep == nullptr || altsep > sep) sep = altsep;
#endif
	// if there is no parent path, just set 'sep'
	// to point to the filename.
	// if there is a parent path, skip the '/' character
	if (sep == nullptr) sep = first;
	else ++sep;

	// return false if the first character of the filename is a .
	if (sep[0] == '.') return false;

	std::cerr << f << "\n";
	return true;
}

string torrent_state(lt::torrent_status const& s)
{
	static char const* state_str[] =
	{ "checking (q)", "checking", "dl metadata"
		, "downloading", "finished", "seeding", "allocating", "checking (r)" };

	if (s.errc) return s.errc.message();
	std::string ret;
	if ((s.flags & lt::torrent_flags::paused) &&
		(s.flags & lt::torrent_flags::auto_managed))
	{
		ret += "queued ";
	}

	if ((s.flags & lt::torrent_flags::upload_mode)) ret += "upload mode";
	else ret += state_str[s.state];

	if (!(s.flags & lt::torrent_flags::auto_managed))
	{
		if (s.flags & lt::torrent_flags::paused)
			ret += " [P]";
		else
			ret += " [F]";
	}
	char buf[10];
	std::snprintf(buf, sizeof(buf), " (%.1f%%)", s.progress_ppm / 10000.f);
	ret += buf;
	return ret;
}

FilesP2PTransport::FilesP2PTransport()
{
}


FilesP2PTransport::~FilesP2PTransport()
{
}




void FilesP2PTransport::CreateSyncTorrentFile(string srcDir, string tFN)
{
	try {
		string creator_str = "libtorrent";
		string comment_str = "Created by filesync application";
		vector<string> web_seeds;
		// you can add any other trackers
		vector<string> trackers = { "udp://tracker.openbittorrent.com:80/announce","udp://tracker.publicbt.com:80/announce" };
		vector<string> collections;
		vector<lt::sha1_hash> similar;
		int pad_file_limit = -1;
		int piece_size = 0;
		// TODO: may be add lt::torrent_flags::seed_mode while torrent creation?

		lt::create_flags_t flags = { };
		string root_cert;
		// created torrent file name
		string outfile = tFN;
		string merklefile;



		lt::file_storage fs;





		// add given list of filenames 
		// to file storage

		lt::add_files(fs, srcDir.c_str(), file_filter, flags);

		
		
		if (fs.num_files() == 0) {
			cerr << "no files specified.\n";
		}

		lt::create_torrent t(fs, piece_size, pad_file_limit, flags);
		int tier = 0;
		for (string const& tr : trackers) {
			t.add_tracker(tr, tier);
			++tier;
		}

		for (string const& ws : web_seeds)
			t.add_url_seed(ws);

		for (string const& c : collections)
			t.add_collection(c);

		for (lt::sha1_hash const& s : similar)
			t.add_similar_torrent(s);

	


		


		// add file piece hashes  
		// to torrent

		auto const num = t.num_pieces();
		
		
		// get the parent directory by trimming last folder name
		srcDir.erase(srcDir.size() - 1, 1);


		lt::set_piece_hashes(t, branch_path(srcDir.c_str())
			, [num](lt::piece_index_t const p) {
			cerr << "\r" << p << "/" << num;
		});
		

		cerr << "\n";
		t.set_creator(creator_str.c_str());
		if (!comment_str.empty()) {
			t.set_comment(comment_str.c_str());
		}

		if (!root_cert.empty()) {
			vector<char> const pem = load_file(root_cert);
			t.set_root_cert(string(&pem[0], pem.size()));
		}

		// create the torrent and print it to stdout
		vector<char> torrent;
		lt::bencode(back_inserter(torrent), t.generate());
		if (!outfile.empty()) {
			fstream out;
			out.exceptions(ifstream::failbit);
			out.open(outfile.c_str(), ios_base::out | ios_base::binary);
			out.write(torrent.data(), torrent.size());
		}
		else {
			cout.write(torrent.data(), torrent.size());
		}

		if (!merklefile.empty()) {
			fstream merkle;
			merkle.exceptions(ifstream::failbit);
			merkle.open(merklefile.c_str(), ios_base::out | ios_base::binary);
			auto const& tree = t.merkle_tree();
			merkle.write(reinterpret_cast<char const*>(tree.data()), tree.size() * 20);
		}

		}
	catch (std::exception& e) {
		std::cerr << "ERROR: " << e.what() << "\n";
	}

}





void FilesP2PTransport::addTorrentSessionForSync(bool sessionType, string dir, string torrentFileName)
{





	lt::add_torrent_params p;
	lt::torrent_handle th;
	lt::torrent_flags_t tft;
	lt::error_code ec;
	
	if (sessionType) {
		dir.erase(dir.size() - 1, 1);
		p.save_path = branch_path(dir);
	}
	else
	{
		p.save_path =dir;

	}
    lt::torrent_info ti(torrentFileName);
	p.ti = make_shared<lt::torrent_info>(ti);
	cout << "adding torrent to session" << endl;
	th=fptSession.add_torrent(p);

	if (sessionType)
	{
		th.set_flags(lt::torrent_flags::seed_mode);
		thSeed = th;
	}
	else
	{
		thDownload = th;
	}


	
	



}

void FilesP2PTransport::PrintInforAboutTorrentFile(string torrentFileName)
{
	try
	{
		int item_limit = 10000;
		int depth_limit = 1000;
		std::vector<char> buf = load_file(torrentFileName.c_str());
		lt::bdecode_node e;
		int pos = -1;
		lt::error_code ec;
		std::cout << "decoding. recursion limit: " << depth_limit
			<< " total item count limit: " << item_limit << "\n";
		int const ret = lt::bdecode(&buf[0], &buf[0] + buf.size(), e, ec, &pos
			, depth_limit, item_limit);
		std::printf("\n\n----- raw info -----\n\n%s\n", print_entry(e).c_str());

		if (ret != 0) {
			std::cerr << "failed to decode: '" << ec.message() << "' at character: " << pos << "\n";
			exit;
		}

		lt::torrent_info const t(e);
		e.clear();
		std::vector<char>().swap(buf);

		// print info about torrent
		std::printf("\n\n----- torrent file info -----\n\n"
			"nodes:\n");
		for (auto const& i : t.nodes())
			std::printf("%s: %d\n", i.first.c_str(), i.second);

		puts("trackers:\n");
		for (auto const& i : t.trackers())
			std::printf("%2d: %s\n", i.tier, i.url.c_str());

		std::stringstream ih;
		ih << t.info_hash();
		std::printf("number of pieces: %d\n"
			"piece length: %d\n"
			"info hash: %s\n"
			"comment: %s\n"
			"created by: %s\n"
			"magnet link: %s\n"
			"name: %s\n"
			"number of files: %d\n"
			"files:\n"
			, t.num_pieces()
			, t.piece_length()
			, ih.str().c_str()
			, t.comment().c_str()
			, t.creator().c_str()
			, make_magnet_uri(t).c_str()
			, t.name().c_str()
			, t.num_files());
		lt::file_storage const& st = t.files();
		for (auto const i : st.file_range())
		{
			auto const first = st.map_file(i, 0, 0).piece;
			auto const last = st.map_file(i, max(std::int64_t(st.file_size(i)) - 1, std::int64_t(0)), 0).piece;
			auto const flags = st.file_flags(i);
			std::stringstream file_hash;
			if (!st.hash(i).is_all_zeros())
				file_hash << st.hash(i);
			std::printf(" %8" PRIx64 " %11" PRId64 " %c%c%c%c [ %5d, %5d ] %7u %s %s %s%s\n"
				, st.file_offset(i)
				, st.file_size(i)
				, ((flags & lt::file_storage::flag_pad_file) ? 'p' : '-')
				, ((flags & lt::file_storage::flag_executable) ? 'x' : '-')
				, ((flags & lt::file_storage::flag_hidden) ? 'h' : '-')
				, ((flags & lt::file_storage::flag_symlink) ? 'l' : '-')
				, static_cast<int>(first)
				, static_cast<int>(last)
				, std::uint32_t(st.mtime(i))
				, file_hash.str().c_str()
				, st.file_path(i).c_str()
				, (flags & lt::file_storage::flag_symlink) ? "-> " : ""
				, (flags & lt::file_storage::flag_symlink) ? st.symlink(i).c_str() : "");
		}
		std::printf("web seeds:\n");
		for (auto const& ws : t.web_seeds())
		{
			std::printf("%s %s\n"
				, ws.type == lt::web_seed_entry::url_seed ? "BEP19" : "BEP17"
				, ws.url.c_str());
		}

		
	}
	catch (std::exception const& e)
	{
		std::cerr << "ERROR: " << e.what() << "\n";
	}
}

void FilesP2PTransport::printCurrentSessionsState()
{

	string torrentState;
	string leaveCond;
	while (leaveCond!="stop") {
		if (thSeed.is_valid()) {
			torrentState = torrent_state(thSeed.status());
			cout << "Seed state:" << torrentState << endl;
		}
		if (thDownload.is_valid()) {
			torrentState = torrent_state(thDownload.status());
			cout << "Download state:" << torrentState << endl;
		}

		cout << " Press enter to refresh status" << endl;
		cin.ignore();
	}
}
