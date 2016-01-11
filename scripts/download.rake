# download.rake
# This deals only with downloading items.

require 'rake'
require 'net/http'
require 'zip'


Zip.on_exists_proc = true
ROOT_DIR = "../"


# Downloads a zip,
# extracts the contents to the destination
# then deletes the zip.
def download_zip(uri, dest_dir)

  print "Downloading ... "

  zip = Net::HTTP.get(uri);

  download_content = "./temp.zip"

  File.open(download_content, 'wb') do |f|
    f.write(zip);
  end

  puts "[Done]"
  print "Unzipping ... "

  zip_file = Zip::File.open(download_content)

  zip_file.each do |f|
    next if f.name =~ /__MACOSX/ or f.name =~ /\.DS_Store/
    begin
      f.extract("#{dest_dir}#{f.name}")
    rescue

    end
  end

  puts "[Done]"
  print "Cleaning up ... "

  # Clean up
  File.delete(download_content)

  puts "[Done]"
end


desc "Download assets contents."
task :download_assets do |t, args|

  puts "Getting Assets"

  asset_path = ROOT_DIR + "assets/"
  asset_uri = "https://dl.dropboxusercontent.com/u/2580523/Wired/assets/assets_01.zip"

  download_zip(asset_uri, asset_path)

end


desc "Download 3rd party libraries."
task :download_3rdparty do |t, args|

  puts "Getting 3rd Party"

  thrid_party_path = ROOT_DIR + "3rdparty/"
  thrid_party_uri = "https://dl.dropboxusercontent.com/u/2580523/Wired/3rdparty/3rdparty_01.zip"

  download_zip(thrid_party_uri, thrid_party_path)

end


desc "Download everything."
task :download_everything do |t, args|

    Rake::Task["download_assets"].execute
    Rake::Task["download_3rdparty"].execute

end
