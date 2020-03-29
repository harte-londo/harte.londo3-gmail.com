import java.io.*;
import java.net.*;
import java.util.Scanner;
import org.jsoup.*;
import org.jsoup.nodes.Document;
import org.jsoup.select.Elements;
import org.jsoup.nodes.Element;
import java.util.ArrayList;
import java.util.Stack;
import java.lang.Integer;
import java.util.StringTokenizer;
import java.util.HashMap;
public class javaPart{
	public static ArrayList<String> visited = new ArrayList<String>();
	public static Stack<Document> pile = new Stack<Document>();
	public static HashMap<String,Integer> finalOut = new HashMap<String,Integer>();

	public static void main(String[] args){	
		String addr = args[0];
		String depth = args[1];
		try{
		Scanner hunIn = new Scanner(System.in);
		File file = new File("output.txt");	
		if(file.exists()){
			file.delete();
		}	
		file.createNewFile();
		FileWriter writer = new FileWriter(file.getAbsoluteFile());
		BufferedWriter bufWrite = new BufferedWriter(writer);
		int dept = Integer.parseInt(depth);
		deepDive(addr,dept);
		ArrayList<String> allWords = new ArrayList<>();
		while(!pile.empty()){
			String a = pile.peek().body().text();//.text();
			String b = pile.pop().select("html").text();
			//String c = pile.pop().outerHtml();
			//String d = pile.pop().body().html();
			String docWordsAlmost = a + " " +  b;// + " " + c;//could get more, but only doing text of body per emails with prof joe.
			String[] docWords = docWordsAlmost.split("\\W+");
			for(String x: docWords){
				System.out.print(x.strip() +  "\n");
				while(!hunIn.hasNextLine()){}
				String response = hunIn.nextLine();
				if(response.contains("*") || response.contains("+") || response.contains("-")){
					if(finalOut.containsKey(x)){
						int count = finalOut.get(x);
						finalOut.replace(x,count + 1);
					}else{
						finalOut.put(x,1);
					}	
				}		
			}
		}
		for(String x: finalOut.keySet()){
			bufWrite.write(String.format("%s : %s \n",x.toString(), finalOut.get(x).toString()));
		}
		bufWrite.close();
		writer.close();	
		System.exit(0);	
		}catch(MalformedURLException e){
		}catch(IOException e){}			
	}

	private static void deepDive(String arg, int depth){
		String nextPlace = ""; 
		if(depth < 0) return;
		if(visited.contains(arg)){return;}
		try{
			Document doc = Jsoup.connect(arg).get();
			pile.push(doc);
			visited.add(arg);
			Elements nextLinks = doc.select("a[href]");
		for(Element next: nextLinks){
			nextPlace = nextLinks.attr("abs:href");
			deepDive(nextPlace,depth-1);
		}
		}catch(IOException e){}
		if(nextPlace == ""){
			return;
		}
	}
}
